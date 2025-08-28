#include <lib/SimpleSearchEngine.h>
#include <filesystem>

#include <gtest/gtest.h>

namespace fs = std::filesystem;

class InvertedIndexTest : public ::testing::Test {
protected:
    InvertedIndex index;
    std::string test_file_path = "data/test_file.txt";
    std::string test_directory_path;

    void SetUp() override {
        fs::path folderPath = fs::absolute(test_file_path).parent_path();

        if (!fs::exists(folderPath)) {
            fs::create_directories(folderPath);
        }

        std::ofstream file(test_file_path);
        if (file.is_open()) {
            file << "This is a test file for unit testing" << std::endl;
            test_directory_path = folderPath.string();
            file.close();
        }
    }

    void TearDown() override {
        std::filesystem::remove(test_file_path);
    }
};


TEST_F(InvertedIndexTest, FilesAmmountTest) {

    index.CreateFullTextIndex(test_directory_path, test_directory_path + "\\testindex.fti");

    EXPECT_EQ(index.GetFilesAmmount(), 1);

    std::filesystem::remove(test_directory_path + "\\testindex.fti");
}

TEST_F(InvertedIndexTest, SizeOfAllFilesTest) {

    index.CreateFullTextIndex(test_directory_path, test_directory_path + "\\testindex.fti");

    EXPECT_EQ(index.GetSizeOfAllFiles(), 8);

    std::filesystem::remove(test_directory_path + "\\testindex.fti");
}

TEST_F(InvertedIndexTest, AverageFileSizeTest) {

    index.CreateFullTextIndex(test_directory_path, test_directory_path + "\\testindex.fti");

    EXPECT_EQ(index.GetAverageFileSize(), 8);

    std::filesystem::remove(test_directory_path + "\\testindex.fti");
}

TEST_F(InvertedIndexTest, UniqFilesAmmountTest) {

    index.CreateFullTextIndex(test_directory_path, test_directory_path + "\\testindex.fti");

    EXPECT_EQ(index.GetUniqFilesAmmount(), 8);

    std::filesystem::remove(test_directory_path + "\\testindex.fti");
}

TEST_F(InvertedIndexTest, IndexContaisWordTest) {

    index.CreateFullTextIndex(test_directory_path, test_directory_path + "\\testindex.fti");

    EXPECT_TRUE(index.IsIndexContaisWord("testing"));

    std::filesystem::remove(test_directory_path + "\\testindex.fti");
}


TEST_F(InvertedIndexTest, ValidSyntaxTest) {
    std::vector<std::string> queries = {
            "for",
            "vector OR list",
            "vector AND list",
            "(for)",
            "(vector OR list)",
            "(vector AND list)",
            "(while OR for) AND vector",
            "for AND and",
            "for AND",
            "vector list",
            "for AND OR list",
            "vector Or list"
    };
    bool queries_answer[] = {
            true,
            true,
            true,
            true,
            true,
            true,
            true,
            true,
            false,
            false,
            false,
            false
    };

    for (int i = 0; i < queries.size(); ++i) {
        ASSERT_EQ(LogicExpression::IsValidSyntax(queries[i]), queries_answer[i]);
    }
}

TEST_F(InvertedIndexTest, ParseExpressionTest) {
    std::vector<std::string> queries = {
            "for",
            "vector OR list",
            "vector AND list",
            "(for)",
            "(vector OR list)",
            "(vector AND list)",
            "(while OR for) AND vector",
            "for AND and"
    };
    std::vector<std::vector<std::string>> queries_answer = {
            {"for"},
            {"vector", "list", "OR"},
            {"vector", "list", "AND"},
            {"for"},
            {"vector", "list", "OR"},
            {"vector", "list", "AND"},
            {"while",  "for",  "OR", "vector", "AND"},
            {"for",    "and",  "AND"}};


    for (int i = 0; i < queries.size(); ++i) {
        LogicExpression le;
        ASSERT_EQ(le.ParseExpression(queries[i]), queries_answer[i]);
    }
}