#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include "LogicExpression.h"
#include <cmath>

static const double kK = 1.3;
static const double kB = 0.75;

struct File {
    size_t file_index = 0;
    size_t word_per_file = 0;
    std::vector<size_t> lines_numbers;
};

struct FileInformation {
    size_t file_size = 0;
    std::string file_name;
    double BM25 = 0;
    std::vector<std::string> words;
    std::vector<size_t> lines;
};

class InvertedIndex {
public:
    void FullTextSearch(const std::string& expression) {
        LogicExpression Exp;
        if (Exp.IsValidSyntax(expression)) {
            std::vector<std::string> reverse_polish_notation = Exp.ParseExpression(expression);
            std::set<std::string>& key_words = Exp.GetKeyWords();

            GetScoreForFiles(key_words);

            std::vector<size_t> valid_files = SolveExpression(reverse_polish_notation);

            std::sort(valid_files.begin(), valid_files.end(),
                      [this](const size_t& first, const size_t& second) -> bool {
                          return files[first].BM25 > files[second].BM25;
                      });

            std::cout << "\n";
            for (const auto& num: valid_files) {
                const auto& file = files[num];
                std::cout << file.file_name << "\t" << file.BM25 << "\n";
                size_t cnt = 0;
                for (const auto& word: file.words) {
                    std::cout << "Key word: " << word << "\nLines: ";
                    while (file.lines[cnt] != 0) {
                        std::cout << file.lines[cnt] << " ";
                        cnt++;
                    }
                    std::cout << "\n";
                    cnt++;
                }
            }

            for (auto& curr: files) {
                curr.BM25 = 0;
                curr.lines.clear();
                curr.words.clear();
            }
        } else {
            std::cerr << "syntax error" << "\n";
        }
    }

    void CreateFullTextIndex(const std::string& directory_path, const std::string& index_file_name) {
        std::cout << "Run CreateFullTextIndex for: " << directory_path << "\n";

        BuildIndex(directory_path);

        std::cout << "Unique elements: " << index.size() << "\n";
        std::cout << "Number of files: " << number_of_files << "\n";
        std::cout << "Average file size: " << size_of_all_files / number_of_files << "\n";
        std::cout << "Size of all files: " << size_of_all_files << "\n";

        SaveIndex(index_file_name);

        std::cout << "Index create at " + index_file_name << "\n";
    }

    void ReadIndex(const std::string& directory_to_read_from) {
        std::ifstream file(directory_to_read_from, std::ios::in | std::ios::binary);

        file.read(reinterpret_cast<char*>(&number_of_files), sizeof(size_t));
        file.read(reinterpret_cast<char*>(&size_of_all_files), sizeof(size_t));

        size_t index_size;
        size_t key_size;
        size_t file_list_size;
        size_t lines_size;

        std::string key;
        file.read(reinterpret_cast<char*>(&index_size), sizeof(index_size));
        std::vector<File> fileList;
        for (size_t i = 0; i < index_size; ++i) {
            file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));

            key.resize(key_size);
            file.read(&key[0], key_size);

            file.read(reinterpret_cast<char*>(&file_list_size), sizeof(file_list_size));
            fileList.resize(file_list_size);

            for (size_t j = 0; j < file_list_size; ++j) {
                file.read(reinterpret_cast<char*>(&fileList[j].file_index), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&fileList[j].word_per_file), sizeof(size_t));
                file.read(reinterpret_cast<char*>(&lines_size), sizeof(size_t));
                fileList[j].lines_numbers.resize(lines_size);

                for (size_t k = 0; k < lines_size; ++k) {
                    file.read(reinterpret_cast<char*>(&fileList[j].lines_numbers[k]), sizeof(size_t));
                }
            }
            index[key] = fileList;
        }

        size_t files_size;
        size_t file_name_size;

        file.read(reinterpret_cast<char*>(&files_size), sizeof(files_size));
        files.resize(files_size);

        for (size_t i = 0; i < files_size; ++i) {
            file.read(reinterpret_cast<char*>(&files[i].file_size), sizeof(size_t));
            file.read(reinterpret_cast<char*>(&file_name_size), sizeof(size_t));
            files[i].file_name.resize(file_name_size);
            file.read(&files[i].file_name[0], file_name_size);
        }

        file.close();
    }

    bool IsIndexContaisWord(const std::string& word) {
        return index.contains(word);
    }

    size_t GetFilesAmmount() {
        return number_of_files;
    }

    size_t GetSizeOfAllFiles() {
        return size_of_all_files;
    }

    double GetAverageFileSize() {
        return size_of_all_files / number_of_files;
    }

    size_t GetUniqFilesAmmount() {
        return index.size();
    }

private:
    void AddWordToIndex(std::string& word, size_t& line_number, size_t& file_number) {
        auto& current_index = index[word];

        if (current_index.empty()) {
            current_index.push_back({file_number, 0, {line_number}});
        }
        if (file_number != current_index.back().file_index) {
            current_index.push_back({file_number, 0, {line_number}});
        }
        if (current_index.back().lines_numbers.back() != line_number) {
            current_index.back().lines_numbers.push_back(line_number);
        }
        current_index.back().word_per_file++;
    };

    void BuildIndex(const std::string& directory_path) {
        size_t file_number = 0;

        for (const auto& entry: std::filesystem::recursive_directory_iterator(directory_path)) {
            if (entry.is_regular_file()) {
                std::string file_name = entry.path().string();
                size_t file_size = IndexFile(file_name, file_number);

                AddNewFile(file_size, file_name);
                file_number++;
            }
        }
    }

    size_t IndexFile(const std::string& file_path, size_t& file_number) {
        std::ifstream file(file_path);
        std::string line;
        size_t line_number = 1;
        size_t words_count = 0;
        std::string word;

        while (std::getline(file, line)) {
            std::istringstream iss(line);

            while (iss >> word) {
                words_count++;
                std::transform(word.begin(), word.end(), word.begin(), ::tolower);

                AddWordToIndex(word, line_number, file_number);
            }
            line_number++;
        }

        return words_count;
    }

    void SaveIndex(const std::string& directory_to_save) {
        std::ofstream output_file(directory_to_save, std::ios::out | std::ios::binary);

        output_file.write(reinterpret_cast<char*>(&number_of_files), sizeof(size_t));
        output_file.write(reinterpret_cast<char*>(&size_of_all_files), sizeof(size_t));

        size_t index_size = index.size();
        size_t key_size;
        size_t file_list_size;
        size_t lines_size;

        output_file.write(reinterpret_cast<char*>(&index_size), sizeof(index_size));

        for (auto& entry: index) {
            key_size = entry.first.size();
            output_file.write(reinterpret_cast<char*>(&key_size), sizeof(key_size));
            output_file.write(entry.first.c_str(), key_size);

            file_list_size = entry.second.size();
            output_file.write(reinterpret_cast<char*>(&file_list_size), sizeof(file_list_size));

            for (auto& file: entry.second) {
                output_file.write(reinterpret_cast<char*>(&file.file_index), sizeof(file.file_index));
                output_file.write(reinterpret_cast<char*>(&file.word_per_file), sizeof(file.word_per_file));

                lines_size = file.lines_numbers.size();
                output_file.write(reinterpret_cast<char*>(&lines_size), sizeof(lines_size));

                for (auto& line: file.lines_numbers) {
                    output_file.write(reinterpret_cast<char*>(&line), sizeof(line));
                }
            }
        }

        size_t files_size = files.size();
        size_t file_name_size;

        output_file.write(reinterpret_cast<char*>(&files_size), sizeof(size_t));
        for (auto& file_inf: files) {
            output_file.write(reinterpret_cast<char*>(&file_inf.file_size), sizeof(file_inf.file_size));
            file_name_size = file_inf.file_name.size();
            output_file.write(reinterpret_cast<char*>(&file_name_size), sizeof(file_name_size));
            output_file.write(reinterpret_cast<const char*>(file_inf.file_name.c_str()), file_name_size);
        }


        output_file.close();
    }

    void AddNewFile(const size_t& file_size, const std::string& file_name) {
        size_of_all_files += file_size;
        number_of_files++;
        files.push_back({file_size, file_name});
    }

    double GetAverageFileSize() const {
        return size_of_all_files / number_of_files;
    }

    double IDF(size_t& files_by_word) const {
        return log((number_of_files - files_by_word + 0.5) / (files_by_word + 0.5) + 1);
    }

    double
    SecondPart(File& current_file) const {
        return (current_file.word_per_file * (kK + 1)) /
               (current_file.word_per_file +
                kK * (1 - kB + kB * (files[current_file.file_index].file_size / GetAverageFileSize())));
    }

    double CalculateBM25(File& file, size_t files_by_word) const {
        return IDF(files_by_word) * SecondPart(file);
    }

    void GetScoreForFiles(std::set<std::string>& key_words) {

        for (auto& word: key_words) {
            if (!index.contains(word)) {
                continue;
            }
            auto& current_index = index[word];

            for (auto file: current_index) {
                auto& current_file = files[file.file_index];

                current_file.BM25 += CalculateBM25(file, current_index.size());
                current_file.words.push_back(word);
                current_file.lines.insert(current_file.lines.end(), file.lines_numbers.begin(),
                                          file.lines_numbers.end());
                current_file.lines.push_back(0);
            }
        }
    }

    std::vector<size_t> SolveExpression(std::vector<std::string>& expression) {
        std::vector<std::vector<size_t>> operands;

        for (auto& elem: expression) {
            if (elem == "OR") {
                if (operands.size() < 2) {
                    throw std::runtime_error("Invalid expression\n");
                }
                std::vector<size_t> result = {};
                std::set_union(operands.back().begin(), operands.back().end(), (operands.end() - 2)->begin(),
                               (operands.end() - 2)->end(),
                               std::back_inserter(result));

                operands.pop_back();
                operands.pop_back();

                operands.push_back(result);
            } else if (elem == "AND") {
                if (operands.size() < 2) {
                    throw std::runtime_error("Invalid expression\n");
                }
                std::vector<size_t> result = {};
                std::set_intersection(operands.back().begin(), operands.back().end(), (operands.end() - 2)->begin(),
                                      (operands.end() - 2)->end(),
                                      std::back_inserter(result));

                operands.pop_back();
                operands.pop_back();

                operands.push_back(result);
            } else {
                operands.push_back({});
                for (auto& file: index[elem]) {
                    operands.back().push_back(file.file_index);
                }
                std::sort(operands.back().begin(), operands.back().end());
            }
        }
        if (operands.size() != 1) {
            throw std::runtime_error("Invalid expression\n");
        }

        return operands.back();
    }

    size_t number_of_files = 0;
    size_t size_of_all_files = 0;
    std::unordered_map<std::string, std::vector<File>> index;
    std::vector<FileInformation> files;
};