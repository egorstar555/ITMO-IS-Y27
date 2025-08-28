#include <lib/ArgParser.h>
#include <HammingArchiver/HammingArchiver.h>
#include <gtest/gtest.h>
#include <sstream>


using namespace ArgumentParser;

class HammingArchiverTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::ofstream test_file1("FILE1.txt");
        test_file1 << "0000000000000000";
        test_file1.close();

        std::ofstream test_file2("FILE2.txt");
        test_file2 << "100000000000000";
        test_file2.close();

        archiver = new HammingArchiver("ARCHIVE.haf");
    }

    void TearDown() override {
        delete archiver;
        std::remove("FILE1.txt");
        std::remove("FILE2.txt");
        std::remove("ARCHIVE.haf");
    }

    HammingArchiver* archiver;
};

TEST_F(HammingArchiverTest, CreateArchiveOneFileTest) {
    std::vector<std::string> file_names;
    file_names.push_back("FILE1.txt");
    archiver->CreateArchive(file_names);

    std::ifstream archive_file("ARCHIVE.haf", std::ios::binary);
    ASSERT_TRUE(archive_file);

    uint64_t name_size_parity;
    archive_file.read(reinterpret_cast<char*>(&name_size_parity), sizeof(name_size_parity));
    ASSERT_EQ(name_size_parity, 10);
    char name_buffer[10];

    archive_file.read(name_buffer, 1);
    ASSERT_EQ(name_buffer[0], '\x02');

    archive_file.read(name_buffer, sizeof(name_buffer) - 1);
    std::string file_name(name_buffer, sizeof(name_buffer) - 1);
    ASSERT_EQ(file_name, "FILE1.txt");

    archive_file.read(name_buffer, 1);
    ASSERT_EQ(name_buffer[0], '\xAE');

    uint64_t file_size_parity;
    archive_file.read(reinterpret_cast<char*>(&file_size_parity), sizeof(file_size_parity));
    ASSERT_EQ(file_size_parity, 18);

    archive_file.read(name_buffer, 1);
    ASSERT_EQ(name_buffer[0], '\x0C');

    std::vector<char> file_content;
    char chunk_buffer[15];

    archive_file.read(chunk_buffer, sizeof(chunk_buffer));
    ASSERT_EQ(std::string(chunk_buffer, sizeof(chunk_buffer)), "000000000000000");

    archive_file.read(chunk_buffer, 1);
    ASSERT_EQ(chunk_buffer[0], '\x1E');

    archive_file.read(chunk_buffer, 1);
    ASSERT_EQ(std::string(chunk_buffer, 1), "0");

    archive_file.read(chunk_buffer, 1);
    ASSERT_EQ(chunk_buffer[0], '\x01');

    archive_file.close();
}

TEST_F(HammingArchiverTest, CreateArchiveTwoFileTest) {
    std::vector<std::string> file_names;
    file_names.push_back("FILE1.txt");
    file_names.push_back("FILE2.txt");
    archiver->CreateArchive(file_names);

    std::ifstream archive_file("ARCHIVE.haf", std::ios::binary);
    ASSERT_TRUE(archive_file);

    uint8_t buffer[90];
    uint8_t expected_output[90] = {
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
            'F', 'I', 'L', 'E', '1', '.', 't', 'x', 't', 0xAE,
            0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
            '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 0x1E,
            '0', 0x01,
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
            'F', 'I', 'L', 'E', '2', '.', 't', 'x', 't', 0xAD,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87,
            '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 0x92,
    };
    archive_file.read(reinterpret_cast<char*>(buffer), 90);
    for (int i = 0; i < 90; ++i) {
        EXPECT_EQ(buffer[i], expected_output[i]);
    }
    archive_file.close();
}

TEST_F(HammingArchiverTest, CheckArchiveTest) {
    std::vector<std::string> file_names;
    file_names.push_back("FILE1.txt");
    file_names.push_back("FILE2.txt");
    archiver->CreateArchive(file_names);

    std::fstream archive_file("ARCHIVE.haf", std::ios::binary | std::ios::in | std::ios::out);
    ASSERT_TRUE(archive_file);

    uint8_t buffer[90];
    archive_file.read(reinterpret_cast<char*>(buffer), 10);
    buffer[9] ^= 1;

    std::streampos read_pos = archive_file.tellg();
    std::streampos write_pos = read_pos - static_cast<std::streamoff>(10);
    archive_file.seekp(write_pos);
    archive_file.write(reinterpret_cast<const char*>(buffer), 10);

    archive_file.close();

    archiver->CheckArchive();

    std::fstream archive_file2("ARCHIVE.haf", std::ios::binary | std::ios::in | std::ios::out);
    ASSERT_TRUE(archive_file);

    uint8_t expected_output[90] = {
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
            'F', 'I', 'L', 'E', '1', '.', 't', 'x', 't', 0xAE,
            0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C,
            '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 0x1E,
            '0', 0x01,
            0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
            'F', 'I', 'L', 'E', '2', '.', 't', 'x', 't', 0xAD,
            0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87,
            '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', 0x92,
    };
    archive_file2.read(reinterpret_cast<char*>(buffer), 90);
    for (int i = 0; i < 90; ++i) {
        EXPECT_EQ(buffer[i], expected_output[i]);
    }
    archive_file2.close();
}

std::vector<std::string> SplitString(const std::string& str) {
    std::istringstream iss(str);

    return {std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>()};
}

void MakeParser(ArgumentParser::ArgParser& parser, std::vector<std::string>& values) {
    parser.AddFlag('c', "create", "Создание нового архива");
    parser.AddStringArgument('f', "file", "Имя файла с архивом");
    parser.AddFlag('l', "list", "Вывести список файлов в архиве");
    parser.AddFlag('x', "extract", "Извлечь файлы из архива (если не указано, то все файлы)");
    parser.AddFlag('a', "append", "Добавить файл в архив");
    parser.AddFlag('d', "delete", "Удалить файл из архива");
    parser.AddFlag('A', "concatenate", "Смерджить два архива");
    parser.AddFlag('k', "check", "Проверить целостность архива");
    parser.AddStringArgument("Files",
                             "Имена файлов передаются свободными аргументами").MultiValue().Positional().StoreValues(
            values);
    parser.AddHelp('h', "help", "Справка о программе");
}

TEST(ArgParserTestSuite, CreateArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --create --file=ARCHIVE.haf FILE1 FILE2 FILE3")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files[1], "FILE2");
    ASSERT_EQ(files[2], "FILE3");
    ASSERT_EQ(files.size(), 3);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_TRUE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, CreateShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -c -f=ARCHIVE.haf FILE1 FILE2 FILE3")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files[1], "FILE2");
    ASSERT_EQ(files[2], "FILE3");
    ASSERT_EQ(files.size(), 3);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_TRUE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ListArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(
            SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --list --file=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_TRUE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ListShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(
            parser.Parse(SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -l -f=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_TRUE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, CheckrchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(
            SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --check --file=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_TRUE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, CheckShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(
            parser.Parse(SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -k -f=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_TRUE(parser.GetFlag("check"));
}


TEST(ArgParserTestSuite, ExtractAllArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(
            SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --extract --file=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_TRUE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ExtractAllShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(
            parser.Parse(SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -x -f=ARCHIVE.haf")));
    ASSERT_EQ(files.size(), 0);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_TRUE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ExtractFileArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --extract --file=ARCHIVE.haf FILE1 FILE2 FILE3")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files[1], "FILE2");
    ASSERT_EQ(files[2], "FILE3");
    ASSERT_EQ(files.size(), 3);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_TRUE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ExtractFileShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -x -f=ARCHIVE.haf FILE1 FILE2 FILE3")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files[1], "FILE2");
    ASSERT_EQ(files[2], "FILE3");
    ASSERT_EQ(files.size(), 3);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_TRUE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, AppendArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --append --file=ARCHIVE.haf FILE1")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files.size(), 1);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_TRUE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, AppendShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(
            SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -a -f=ARCHIVE.haf FILE1")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files.size(), 1);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_TRUE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, DeleteArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --delete --file=ARCHIVE.haf FILE1")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files.size(), 1);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_TRUE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, DeleteShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(
            SplitString("C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -d -f=ARCHIVE.haf FILE1")));
    ASSERT_EQ(files[0], "FILE1");
    ASSERT_EQ(files.size(), 1);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_TRUE(parser.GetFlag("delete"));
    ASSERT_FALSE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}


TEST(ArgParserTestSuite, ConcatenateArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe --concatenate ARCHIVE1 ARCHIVE2 --file=ARCHIVE3.haf")));
    ASSERT_EQ(files[0], "ARCHIVE1");
    ASSERT_EQ(files[1], "ARCHIVE2");
    ASSERT_EQ(files.size(), 2);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE3.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_TRUE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, ConcatenateShortArchiveTest) {
    ArgParser parser("My Parser");
    std::vector<std::string> files;
    MakeParser(parser, files);

    ASSERT_TRUE(parser.Parse(SplitString(
            "C:\\cppproj\\labwork6\\cmake-build-debug\\bin\\hamarc.exe -A ARCHIVE1 ARCHIVE2 -f=ARCHIVE3.haf")));
    ASSERT_EQ(files[0], "ARCHIVE1");
    ASSERT_EQ(files[1], "ARCHIVE2");
    ASSERT_EQ(files.size(), 2);
    ASSERT_EQ(parser.GetStringValue("file"), "ARCHIVE3.haf");
    ASSERT_FALSE(parser.GetFlag("create"));
    ASSERT_FALSE(parser.GetFlag("list"));
    ASSERT_FALSE(parser.GetFlag("extract"));
    ASSERT_FALSE(parser.GetFlag("append"));
    ASSERT_FALSE(parser.GetFlag("delete"));
    ASSERT_TRUE(parser.GetFlag("concatenate"));
    ASSERT_FALSE(parser.GetFlag("check"));
}

TEST(ArgParserTestSuite, EmptyTest) {
    ArgParser parser("My Empty Parser");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
}


TEST(ArgParserTestSuite, StringTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}

TEST(ArgParserTestSuite, ShortNameTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument('p', "param1");

    ASSERT_TRUE(parser.Parse(SplitString("app -p=value1")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}

TEST(ArgParserTestSuite, DefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1").Default("value1");

    ASSERT_TRUE(parser.Parse(SplitString("app")));
    ASSERT_EQ(parser.GetStringValue("param1"), "value1");
}

TEST(ArgParserTestSuite, NoDefaultTest) {
    ArgParser parser("My Parser");
    parser.AddStringArgument("param1");

    ASSERT_FALSE(parser.Parse(SplitString("app")));
}

TEST(ArgParserTestSuite, StoreValueTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1")));
    ASSERT_EQ(value, "value1");
}

TEST(ArgParserTestSuite, MultiStringTest) {
    ArgParser parser("My Parser");
    std::string value;
    parser.AddStringArgument("param1").StoreValue(value);
    parser.AddStringArgument('a', "param2");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=value1 --param2=value2")));
    ASSERT_EQ(parser.GetStringValue("param2"), "value2");
}

TEST(ArgParserTestSuite, IntTest) {
    ArgParser parser("My Parser");
    parser.AddIntArgument("param1");

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=100500")));
    ASSERT_EQ(parser.GetIntValue("param1"), 100500);
}

TEST(ArgParserTestSuite, MultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    parser.AddIntArgument('p', "param1").MultiValue().StoreValues(int_values);

    ASSERT_TRUE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
    ASSERT_EQ(parser.GetIntValue("param1", 0), 1);
    ASSERT_EQ(int_values[1], 2);
    ASSERT_EQ(int_values[2], 3);
}

TEST(ArgParserTestSuite, MinCountMultiValueTest) {
    ArgParser parser("My Parser");
    std::vector<int> int_values;
    size_t MinArgsCount = 10;
    parser.AddIntArgument('p', "param1").MultiValue(MinArgsCount).StoreValues(int_values);

    ASSERT_FALSE(parser.Parse(SplitString("app --param1=1 --param1=2 --param1=3")));
}

TEST(ArgParserTestSuite, FlagTest) {
    ArgParser parser("My Parser");
    parser.AddFlag('f', "flag1");

    ASSERT_TRUE(parser.Parse(SplitString("app --flag1")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
}

TEST(ArgParserTestSuite, FlagsTest) {
    ArgParser parser("My Parser");
    bool flag3;
    parser.AddFlag('a', "flag1");
    parser.AddFlag('b', "flag2").Default(true);
    parser.AddFlag('c', "flag3").StoreValue(flag3);

    ASSERT_TRUE(parser.Parse(SplitString("app -a -c")));
    ASSERT_TRUE(parser.GetFlag("flag1"));
    ASSERT_TRUE(parser.GetFlag("flag2"));
    ASSERT_TRUE(flag3);
}

TEST(ArgParserTestSuite, PositionalArgTest) {
    ArgParser parser("My Parser");
    std::vector<int> values;
    parser.AddIntArgument("Param1").MultiValue(1).Positional().StoreValues(values);

    ASSERT_TRUE(parser.Parse(SplitString("app 1 2 3 4 5")));
    ASSERT_EQ(values[0], 1);
    ASSERT_EQ(values[2], 3);
    ASSERT_EQ(values.size(), 5);
}

TEST(ArgParserTestSuite, HelpTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");

    ASSERT_TRUE(parser.Parse(SplitString("app --help")));
    ASSERT_TRUE(parser.Help());
}

TEST(ArgParserTestSuite, HelpStringTest) {
    ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
    parser.AddFlag('s', "flag1", "Use some logic").Default(true);
    parser.AddFlag('p', "flag2", "Use some logic");
    parser.AddIntArgument("numer", "Some Number");


    ASSERT_TRUE(parser.Parse(SplitString("app --help")));
    // Проверка закоментирована намеренно. Ождиается, что результат вызова функции будет приблизительно такой же,
    // но не с точностью до символа

    // ASSERT_EQ(
    //     parser.HelpDescription(),
    //     "My Parser\n"
    //     "Some Description about program\n"
    //     "\n"
    //     "-i,  --input=<string>,  File path for input file [repeated, min args = 1]\n"
    //     "-s,  --flag1,  Use some logic [default = true]\n"
    //     "-p,  --flag2,  Use some logic\n"
    //     "     --number=<int>,  Some Number\n"
    //     "\n"
    //     "-h, --help Display this help and exit\n"
    // );
}
