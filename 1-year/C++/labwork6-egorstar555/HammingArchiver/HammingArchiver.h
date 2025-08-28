#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

class HammingArchiver {
public:
    HammingArchiver(const std::string& name);

    void CreateArchive(std::vector<std::string>& file_names);

    struct Header {
        std::string file_name = "";
    };

    bool GetHeader(Header& header, std::fstream& file);

    void ListFiles();

    void ExtractFiles(const std::vector<std::string>& filenames);

    void AppendFile(const std::string& file_name);

    void AppendDirectory(const std::string& directory_name);

    void DeleteFile(const std::string& del_name);

    void ConcatenateArchives(const std::string& archive_name_1, const std::string& archive_name_2);

    void CheckArchive();

    void CheckBlockParity(uint8_t* data, uint32_t data_size_byte, std::fstream& arc_file);

private:
    std::string name_;
    static const int32_t kSizeBlockByte = 15;
    static const int32_t kSizeBlockBit = kSizeBlockByte * 8;
    static const int32_t kNumParityBit = 8;
    static const int32_t kSizeParityByte = kNumParityBit / 8;
    static const int32_t kSizeBlockWithParity = kSizeBlockByte + kSizeParityByte;
    static const int32_t kFirstParityPosition = 3;
    static const uint8_t kLastParity = 7;
    static const uint8_t kBitByByte = 8;
    std::vector<uint16_t> num_bits;
    std::unordered_map<uint16_t, uint16_t> num_bits_revers;
    bool is_not_fix = false;
    int32_t num_fixed_errors = 0;

    void GetFileNames(const std::filesystem::path& path, std::vector<std::string>& file_names);

    uint64_t CalculateSizeWithParity(uint64_t size_block);

    uint8_t CalculateParityBits(uint8_t* data, uint32_t data_size_bit);
};

