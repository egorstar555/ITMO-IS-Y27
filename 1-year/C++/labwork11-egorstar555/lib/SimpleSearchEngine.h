#pragma once

#include "InvertedIndex.h"

void RunSearch(const std::string& index_file_name, InvertedIndex& index) {
    std::cout << "Read index: " + index_file_name << "\n";
    index.ReadIndex(index_file_name);

    std::cout << "Enter search expression (br - exit):" << "\n";

    std::string a;
    std::getline(std::cin, a);
    while (a != "br") {
        try {
            index.FullTextSearch(a);
        } catch (std::exception& e) {
            std::cout << e.what();
        }

        std::cout << "\n" << "Enter search expression (br - exit):" << "\n";
        std::getline(std::cin, a);
    }
}

void ParseCommandLineArguments(int argc, char* argv[], InvertedIndex& index) {
    std::vector<std::string> args(argv, argv + argc);

    for (size_t i = 1; i < args.size(); ++i) {
        if ((args[i] == "-s" || args[i] == "--search") && i + 1 < args.size()) {
            try {
                RunSearch(args[i + 1], index);
            } catch (std::exception& e) {
                std::cout << "Invalid directory" << "\n";
            }
            break;
        } else if ((args[i] == "-c" || args[i] == "--create") && i + 2 < args.size()) {
            try {
                index.CreateFullTextIndex(args[i + 1], args[i + 2]);
            } catch (std::exception& e) {
                std::cout << "Invalid directory" << "\n";
            }

            break;
        } else {
            std::cerr << "Invalid arguments" << "\n";
            std::cerr << "To create index: -c <folder> <fileIndexName>" << "\n";
            std::cerr << "To search: -s <fileIndexName>" << "\n";
            break;
        }
    }
}