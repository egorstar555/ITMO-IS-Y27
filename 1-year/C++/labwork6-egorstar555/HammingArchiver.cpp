#include "HammingArchiver/HammingArchiver.h"

HammingArchiver::HammingArchiver(const std::string& name)
        : name_{name} {
    for (int i = kFirstParityPosition; i < (kSizeBlockBit + kNumParityBit); i++) {
        if ((i & (i - 1)) != 0) {
            num_bits_revers[i] = num_bits.size();
            num_bits.push_back(i);
        }
    }
}

void HammingArchiver::CreateArchive(std::vector<std::string>& file_names) {
    std::ofstream out_file(name_, std::ios::binary | std::ios::trunc);
    if (!out_file) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }
    out_file.close();
    for (const auto& file_name: file_names) {
        AppendFile(file_name);
    }
}

bool HammingArchiver::GetHeader(Header& header, std::fstream& file) {
    uint64_t name_size;
    uint64_t num_chunks;
    uint64_t remaining_size;

    char data_name[kSizeBlockWithParity];
    if (file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size))) {
        file.read(data_name, kSizeParityByte);
        header.file_name = "";
        num_chunks = name_size / kSizeBlockWithParity;
        remaining_size = name_size % kSizeBlockWithParity;

        for (int i = 0; i < num_chunks; i++) {
            file.read(data_name, kSizeBlockWithParity);
            header.file_name.append(data_name, kSizeBlockWithParity - kSizeParityByte);
        }
        if (remaining_size > 0) {
            file.read(data_name, remaining_size);
            header.file_name.append(data_name, remaining_size - kSizeParityByte);
        }

        return true;
    }

    return false;
}

void HammingArchiver::ListFiles() {
    std::fstream file(name_, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }
    uint64_t size_file;
    char data_name[kSizeBlockWithParity];

    Header header;
    while (GetHeader(header, file)) {
        std::cout << header.file_name << "\n";

        file.read(reinterpret_cast<char*>(&size_file), sizeof(size_file));
        file.read(data_name, kSizeParityByte);

        std::streampos current_pos = file.tellg();
        file.seekg(current_pos + static_cast<std::streamoff>(size_file));
    }
}

void HammingArchiver::ExtractFiles(const std::vector<std::string>& filenames) {
    std::fstream arc_file(name_, std::ios::in | std::ios::binary);
    if (!arc_file) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }
    uint64_t size_file;
    char data_name[kSizeBlockWithParity];
    char output_data[kSizeBlockWithParity];
    uint64_t num_chunks;
    uint64_t remaining_size;
    std::string file_name;
    Header header;
    while (GetHeader(header, arc_file)) {
        arc_file.read(reinterpret_cast<char*>(&size_file), sizeof(size_file));
        arc_file.read(data_name, kSizeParityByte);

        auto it = std::find(filenames.begin(), filenames.end(), header.file_name);
        if (filenames.empty() || it != filenames.end()) {
            std::ofstream out_file(header.file_name, std::ios::binary | std::ios::out);
            if (!out_file) {
                std::cerr << "The file could not be opened." << std::endl;
                return;
            }
            num_chunks = size_file / kSizeBlockWithParity;
            remaining_size = size_file % kSizeBlockWithParity;

            for (int i = 0; i < num_chunks; i++) {
                arc_file.read(output_data, kSizeBlockWithParity);
                out_file.write(reinterpret_cast<const char*>(&output_data),
                               kSizeBlockWithParity - kSizeParityByte);
            }
            if (remaining_size > 0) {
                arc_file.read(output_data, remaining_size);
                out_file.write(reinterpret_cast<const char*>(&output_data), remaining_size - kSizeParityByte);
            }
            out_file.close();
        } else {
            std::streampos current_pos = arc_file.tellg();
            arc_file.seekg(current_pos + static_cast<std::streamoff>(size_file));
        }
    }
    arc_file.close();
}

void HammingArchiver::AppendFile(const std::string& file_name) {
    uint64_t name_size = file_name.size();
    uint64_t name_size_parity = CalculateSizeWithParity(name_size);

    std::ifstream app_file(file_name, std::ios::in | std::ios::binary | std::ios::ate);
    if (!app_file) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }
    std::streampos file_size = app_file.tellg();

    uint64_t file_size_parity = CalculateSizeWithParity(file_size);

    std::ofstream out_file(name_, std::ios::binary | std::ios::app);
    if (!out_file) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }

    uint8_t parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(&name_size_parity),
                                              sizeof(name_size_parity) * kBitByByte);
    out_file.write(reinterpret_cast<const char*>(&name_size_parity), sizeof(name_size_parity));
    out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));

    char output_data[kSizeBlockByte];
    uint64_t num_chunks = name_size / kSizeBlockByte;
    uint64_t remaining_size = name_size % kSizeBlockByte;

    for (int i = 0; i < num_chunks; i++) {
        file_name.copy(output_data, kSizeBlockByte, i * kSizeBlockByte);
        parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(output_data), kSizeBlockBit);
        out_file.write(reinterpret_cast<const char*>(&output_data), kSizeBlockByte);
        out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));
    }
    if (remaining_size > 0) {
        file_name.copy(output_data, remaining_size, num_chunks * kSizeBlockByte);
        parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(output_data), remaining_size * kBitByByte);
        out_file.write(reinterpret_cast<const char*>(&output_data), remaining_size);
        out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));
    }

    out_file.write(reinterpret_cast<const char*>(&file_size_parity), sizeof(file_size_parity));
    parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(&file_size_parity),
                                      sizeof(file_size_parity) * kBitByByte);
    out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));

    app_file.seekg(0, std::ios::beg);
    while (app_file.read(output_data, kSizeBlockByte)) {
        parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(output_data), kSizeBlockBit);
        out_file.write(reinterpret_cast<const char*>(&output_data), kSizeBlockByte);
        out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));
    }

    std::streamsize bytes_read = app_file.gcount();
    if (bytes_read > 0) {
        parity_bits = CalculateParityBits(reinterpret_cast<uint8_t*>(output_data), bytes_read * kBitByByte);
        out_file.write(reinterpret_cast<const char*>(&output_data), bytes_read);
        out_file.write(reinterpret_cast<const char*>(&parity_bits), sizeof(parity_bits));
    }
    out_file.close();
    app_file.close();
}

void HammingArchiver::GetFileNames(const std::filesystem::path& path, std::vector<std::string>& file_names) {
    for (const auto& entry: std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            file_names.push_back(entry.path().string());
        } else if (entry.is_directory()) {
            GetFileNames(entry.path(), file_names);
        }
    }
}

void HammingArchiver::AppendDirectory(const std::string& directory_name) {
    std::vector<std::string> file_names;
    GetFileNames(directory_name, file_names);
    CreateArchive(file_names);
}

void HammingArchiver::DeleteFile(const std::string& del_name) {
    std::fstream arc(name_, std::ios::binary | std::ios::in | std::ios::out);
    if (!arc) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }
    uint64_t name_size;
    uint64_t size_file;
    char data_name[kSizeBlockWithParity];
    std::streampos position;
    std::streamsize num_bytes_del;
    position = arc.tellg();
    Header header;
    while (GetHeader(header, arc)) {
        arc.read(reinterpret_cast<char*>(&size_file), sizeof(size_file));
        arc.read(data_name, kSizeParityByte);
        if (header.file_name == del_name) {
            num_bytes_del = static_cast<std::streamsize>(name_size + sizeof(name_size) + kSizeParityByte +
                                                         size_file + sizeof(size_file) + kSizeParityByte);
            break;
        }

        std::streampos current_pos = arc.tellg();
        arc.seekg(current_pos + static_cast<std::streamoff>(size_file));
        position = arc.tellg();
    }
    if (header.file_name == del_name) {
        arc.seekg(position + num_bytes_del);
        std::string tmp_name = "not_used_name"; //std::tmpnam(nullptr);
        std::ofstream tmp_file(tmp_name, std::ios::binary | std::ios::trunc);
        if (!tmp_file) {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }

        tmp_file << arc.rdbuf();
        tmp_file.close();

        std::fstream in_file(tmp_name, std::ios::binary | std::ios::in);
        if (!in_file) {
            std::cerr << "Failed to open the file." << std::endl;
            return;
        }
        arc.clear();
        arc.seekp(position);
        arc << in_file.rdbuf();
        if (!arc.good()) {
            arc.clear();
            arc.seekp(position);
        }
        num_bytes_del = arc.tellp();
        arc.close();
        in_file.close();
        std::filesystem::remove(tmp_name);
        std::filesystem::resize_file(name_, num_bytes_del);
    }
}

void HammingArchiver::ConcatenateArchives(const std::string& archive_name_1,
                                          const std::string& archive_name_2) {
    std::ofstream arc_new(name_, std::ios::binary);
    std::ifstream arc_1(archive_name_1, std::ios::binary);
    std::ifstream arc_2(archive_name_2, std::ios::binary);
    if (!arc_new || !arc_1 || !arc_2) {
        std::cerr << "The file could not be opened." << std::endl;
        return;
    }
    arc_new << arc_1.rdbuf();
    arc_new << arc_2.rdbuf();

    arc_new.close();
    arc_1.close();
    arc_2.close();
}

void HammingArchiver::CheckArchive() {
    std::fstream arc_file(name_, std::ios::binary | std::ios::in | std::ios::out);
    if (!arc_file) {
        std::cerr << "Failed to open the file." << std::endl;
        return;
    }

    uint64_t name_size;
    uint64_t size_file;
    char data_name[kSizeBlockWithParity];
    char output_data[kSizeBlockWithParity];
    uint64_t num_chunks;
    uint64_t remaining_size;

    while (arc_file.read(reinterpret_cast<char*>(&name_size), sizeof(name_size))) {
        memcpy(data_name, &name_size, sizeof(name_size));
        arc_file.read(reinterpret_cast<char*>(&data_name[sizeof(name_size)]), kSizeParityByte);
        CheckBlockParity(reinterpret_cast<uint8_t*>(data_name), sizeof(name_size) + kSizeParityByte, arc_file);

        num_chunks = name_size / kSizeBlockWithParity;
        remaining_size = name_size % kSizeBlockWithParity;
        for (int i = 0; !is_not_fix && i < num_chunks; i++) {
            arc_file.read(data_name, kSizeBlockWithParity);
            CheckBlockParity(reinterpret_cast<uint8_t*>(data_name), kSizeBlockWithParity, arc_file);
        }
        if (!is_not_fix && remaining_size > 0) {
            arc_file.read(data_name, remaining_size);
            CheckBlockParity(reinterpret_cast<uint8_t*>(data_name), remaining_size, arc_file);
        }

        arc_file.read(reinterpret_cast<char*>(&size_file), sizeof(size_file));
        memcpy(data_name, &size_file, sizeof(size_file));
        arc_file.read(reinterpret_cast<char*>(&data_name[sizeof(size_file)]), kSizeParityByte);
        CheckBlockParity(reinterpret_cast<uint8_t*>(data_name), sizeof(name_size) + kSizeParityByte, arc_file);

        num_chunks = size_file / kSizeBlockWithParity;
        remaining_size = size_file % kSizeBlockWithParity;

        for (int i = 0; !is_not_fix && i < num_chunks; i++) {
            arc_file.read(output_data, kSizeBlockWithParity);
            CheckBlockParity(reinterpret_cast<uint8_t*>(output_data), kSizeBlockWithParity, arc_file);
        }
        if (!is_not_fix && remaining_size > 0) {
            arc_file.read(output_data, remaining_size);
            CheckBlockParity(reinterpret_cast<uint8_t*>(output_data), remaining_size, arc_file);
        }
    }
    arc_file.close();
    if (is_not_fix) {
        std::cerr << "Uncorrectable parity error detected\n";
        return;
    }
    if (num_fixed_errors > 0) {
        std::cerr << "Fixed parity errors: " << num_fixed_errors << "\n";
    }
}

uint64_t HammingArchiver::CalculateSizeWithParity(uint64_t size_block) {
    return size_block +
           ((size_block / kSizeBlockByte) * kSizeParityByte) +
           (((size_block % kSizeBlockByte) > 0) ? kSizeParityByte : 0);
}

uint8_t HammingArchiver::CalculateParityBits(uint8_t* data, uint32_t data_size_bit) {
    uint8_t parity_bits = {};
    int all_parity_bit = 0;
    for (int i = 0; i < data_size_bit; ++i) {
        if (data[i / kBitByByte] & (1 << (kLastParity - i % kBitByByte))) {
            for (int j = 0; j < kLastParity; ++j) {
                if ((num_bits[i] & (1 << j))) {
                    parity_bits ^= (1 << j);
                }
            }
            all_parity_bit ^= 1;
        }
    }
    if (all_parity_bit) {
        parity_bits ^= (1 << kLastParity);
    }

    return parity_bits;
}

void HammingArchiver::CheckBlockParity(uint8_t* data, uint32_t data_size_byte, std::fstream& arc_file) {
    uint8_t parity_bits = CalculateParityBits(data, (data_size_byte - kSizeParityByte) * kBitByByte);
    uint8_t arc_parity_bits = data[data_size_byte - kSizeParityByte];
    if (parity_bits != arc_parity_bits) {
        uint8_t syndrome = parity_bits ^ arc_parity_bits;
        if (syndrome & (1 << kLastParity)) {
            syndrome ^= (1 << kLastParity);
            if ((syndrome & (syndrome - 1)) == 0) {
                data[data_size_byte - kSizeParityByte] = parity_bits;
            } else {
                uint16_t num_bad_bit = num_bits_revers[syndrome];
                data[num_bad_bit / kBitByByte] ^= (1 << (kLastParity - num_bad_bit % kBitByByte));
            }
            std::streampos read_pos = arc_file.tellg();
            std::streampos write_pos = read_pos - static_cast<std::streamoff>(data_size_byte);
            arc_file.seekp(write_pos);
            arc_file.write(reinterpret_cast<const char*>(data), data_size_byte);
            ++num_fixed_errors;
        } else {
            is_not_fix = true;
        }
    }
}
