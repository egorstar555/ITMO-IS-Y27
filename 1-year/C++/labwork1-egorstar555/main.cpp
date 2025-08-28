#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>

void PrintHead(uint64_t lines, std::ifstream& my_file, char delimiter) { // печать заданного количества строк с начала
    uint64_t lines_count = 0;
    char c;
    while (lines_count < lines && my_file.get(c)) {
        std::cout << c;
        if (c == delimiter) {
            lines_count++;
        }
    }
}

void PrintTail(uint64_t lines, std::ifstream& my_file, char delimiter) { // печать строк с конца
    uint64_t lines_count = 0;
    char c;

    while (my_file.get(c)) { // cчитаем количество строк
        if (c == delimiter) {
            lines_count++;
        }
    }
    lines_count++;

    my_file.clear();
    my_file.seekg(0, std::ios_base::beg);

    int64_t skip_lines = lines_count - lines;
    uint64_t temp_lines = 0;

    if (skip_lines > 0) {
        while (temp_lines != skip_lines && my_file.get(c)) { // пропускаем строки сначала, которые не нужно печатать
            if (c == delimiter) {
                temp_lines++;
            }
        }
    }
    while (my_file.get(c)) {
        std::cout << c;
    }
}

void PrintFileContent(char* file_name, uint64_t lines, bool is_tail, char delimiter) { // печать содержимого файла
    std::ifstream my_file(file_name);
    if (!my_file.is_open()) {
        std::cerr << "No such filename or directory :(";
        return;
    }
    char c;
    if (!is_tail) { // печатаем строки сначала
        PrintHead(lines, my_file, delimiter);
    } else { // печатаем строки с конца
        PrintTail(lines, my_file, delimiter);
    }
}

bool MatchlinesStr(char* a, char* b) { // сравнение двух строк
    uint64_t len_b = std::strlen(b);
    for (int64_t i = 0; i < len_b; i++) {
        if (a[i] != b[i]) {
            return false;
        }
    }

    return true;
}

struct Arguments { // структура аргументов для парсера
    char* file_name;
    uint64_t lines = std::numeric_limits<uint64_t>::max(); // 18446744073709551615
    bool is_tail = false;
    char delimiter= '\n';
    bool is_error = false;
};

Arguments ParserArguments(int argc,char** argv){ // парсер аргументов командной строки
    Arguments my_arguments;
    char delimiter_char[] = "--delimiter="; 
    
    char line_char[] = "--lines="; 
    

    if (argc < 2) { // не задано имя файла
        std::cerr << "No filename";
        my_arguments.is_error = true;

        return my_arguments;
    }
    int i = 0;
    char* p_end{};
    const uint8_t LINES_CHAR_LEN = std::strlen("--lines=");
    const uint8_t DELIMITER_CHAR_LEN = std::strlen("--delimiter=");
    while (i < argc) {
        if ((!strcmp(argv[i], "-l") && (i + 1 < argc))) { // пришло количество строк в коротком параметре -l
            my_arguments.lines = std::strtoll(argv[i + 1], &p_end, 10);
            if (my_arguments.lines <= 0) {
                std::cerr << "Invalid input: the number of rows can only be a positive number" << "\n";
                my_arguments.is_error = true;
                return my_arguments;
            }
            i++;
        } else if (MatchlinesStr(argv[i], line_char)) { // пришло количество строк в длинном параметре --lines=
            char* pointer = argv[i];
            pointer += LINES_CHAR_LEN;
            my_arguments.lines = std::strtoull(pointer, &p_end, 10);
            if (my_arguments.lines <= 0) {
                std::cerr << "Invalid input: the number of rows can only be a positive number" << "\n";
                my_arguments.is_error = true;
                return my_arguments;
            }
        } else if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--tail")) { // нужно выводить последние строки
            my_arguments.is_tail = true;
        } else if (strlen(argv[i]) > DELIMITER_CHAR_LEN && MatchlinesStr(argv[i], delimiter_char)) { // пришел длинный --delimiter=
            if (argv[i][DELIMITER_CHAR_LEN] != '\\') {
                my_arguments.delimiter= argv[i][DELIMITER_CHAR_LEN];
            } else if (argv[i][DELIMITER_CHAR_LEN] == '\\' && argv[i][DELIMITER_CHAR_LEN + 1] == 't') {
                my_arguments.delimiter= '\t';
            }
        } else if (!strcmp(argv[i], "-d") && (i + 1 < argc)) { // пиршел короткий -d
            if (!strcmp(argv[i + 1], "\\t")) {
                my_arguments.delimiter= '\t';
            } else if (argv[i + 1][0] != '\\') {
                my_arguments.delimiter= argv[i + 1][0];
            }
            i++;
        } else { // если не параметры, то имя файла
            my_arguments.file_name = argv[i];
        }
        i++;
    }

    return my_arguments;
}

int main(int argc, char** argv) {
    Arguments my_args = ParserArguments(argc, argv);
    if (!my_args.is_error){
        PrintFileContent(my_args.file_name, my_args.lines, my_args.is_tail, my_args.delimiter);
    }
    
    return 0;
}
