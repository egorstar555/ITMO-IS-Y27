#include <functional>
#include <lib/ArgParser.h>
#include "HammingArchiver/HammingArchiver.h"
#include <iostream>

void ArchiveActions(ArgumentParser::ArgParser& parser, std::vector<std::string>& files) {
    HammingArchiver archive(parser.GetStringValue("file"));

    if (parser.GetFlag("create")) {
        // Создание нового архива: --create --file=ARCHIVE.haf FILE1.txt FILE2.txt
        archive.CreateArchive(files);

        return;
    }
    if (parser.GetFlag("list")) {
        // Вывести список файлов в архиве: --list --file=ARCHIVE.haf
        archive.ListFiles();

        return;
    }
    if (parser.GetFlag("extract")) {
        // Извлечь файлы из архива (если не указано, то все файлы) --extract --file=ARCHIVE.haf FILE1.txt FILE2.txt
        archive.ExtractFiles(files);

        return;
    }
    if (parser.GetFlag("append")) {
        // Добавить файл в архив --append --file=ARCHIVE.haf FILE1
        archive.AppendFile(files[0]);

        return;
    }
    if (parser.GetFlag("folder")) {
        // Добавить папку в архив --folder --file=ARCHIVE.haf TestFolder
        archive.AppendDirectory(files[0]);

        return;
    }
    if (parser.GetFlag("delete")) {
        // Удалить файл из архива --delete --file=ARCHIVE.haf FILE1.txt
        archive.DeleteFile(files[0]);

        return;
    }
    if (parser.GetFlag("concatenate")) {
        // Смерджить два архива --concatenate ARCHIVE.haf ARCHIVE1.haf --file=ARCHIVE2.haf
        archive.ConcatenateArchives(files[0], files[1]);

        return;
    }
    if (parser.GetFlag("check")) {
        // Проверить целостность архива --check --file=ARCHIVE.haf
        archive.CheckArchive();

        return;
    }
}

void MakeParser(ArgumentParser::ArgParser& parser, std::vector<std::string>& values) {
    parser.AddFlag('c', "create", "Создание нового архива");
    parser.AddStringArgument('f', "file", "Имя файла с архивом");
    parser.AddFlag('l', "list", "Вывести список файлов в архиве");
    parser.AddFlag('x', "extract", "Извлечь файлы из архива (если не указано, то все файлы)");
    parser.AddFlag('a', "append", "Добавить файл в архив");
    parser.AddFlag('o', "folder", "Добавить папку в архив");
    parser.AddFlag('d', "delete", "Удалить файл из архива");
    parser.AddFlag('A', "concatenate", "Смерджить два архива");
    parser.AddFlag('k', "check", "Проверить целостность архива");
    parser.AddStringArgument("Files",
                             "Имена файлов передаются свободными аргументами").MultiValue().Positional().StoreValues(
            values);
    parser.AddHelp('h', "help", "Справка о программе");
}

int main(int argc, char** argv) {
    std::vector<std::string> files;

    ArgumentParser::ArgParser parser("HamArc parser");
    MakeParser(parser, files);

    if (!parser.Parse(argc, argv)) {
        std::cout << "Wrong argument" << std::endl;
        std::cout << parser.HelpDescription() << std::endl;
        return 1;
    }

    if (parser.Help()) {
        std::cout << parser.HelpDescription() << std::endl;
        return 0;
    }

    ArchiveActions(parser, files);

    return 0;
}
