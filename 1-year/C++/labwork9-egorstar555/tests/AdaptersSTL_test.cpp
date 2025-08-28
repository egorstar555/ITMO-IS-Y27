#include <lib/AdaptersSTL.h>
#include "lib/transform.h"
#include "lib/filter.h"
#include "lib/take.h"
#include "lib/reverse.h"
#include "lib/drop.h"
#include "lib/keys.h"
#include "lib/values.h"
#include <queue>
#include <gtest/gtest.h>
#include <sstream>

using namespace ranges;

TEST(TransformTest, DereferenceOperator) {
    // Arrange
    std::vector<int> vec = {1, 2, 3};
    auto func = [](int val) { return val * 2; };
    TransformIterator<std::vector<int>::const_iterator, decltype(func)> it(vec.begin(), func);

    // Act
    auto result = *it;

    // Assert
    EXPECT_EQ(result, 2);
}

TEST(TransformTest, GetFunction) {
    // Arrange
    auto func = [](int val) { return val * val; };
    view::TransformAdaptor<decltype(func)> adaptor(func);

    // Act
    auto resultFunc = adaptor.GetFunction();

    // Assert
    EXPECT_EQ(resultFunc(3), 9);
}

TEST(TransformTest, TransformWholeVector) {
    // Arrange
    std::vector<int> vec = {1, 2, 3};
    auto func = [](int val) { return val * 2; };

    // Act
    auto transformedRange = vec | view::transform(func);
    std::vector<int> transformedVec(transformedRange.begin(), transformedRange.end());

    // Assert
    EXPECT_EQ(transformedVec.size(), vec.size());

    for (size_t i = 0; i < vec.size(); ++i) {
        EXPECT_EQ(transformedVec[i], vec[i] * 2);
    }
}


TEST(FilterTest, IncrementOperatorTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5};
    auto pred = [](int val) { return val % 2 == 0; };
    auto it = MakeFilterIterator(data.begin(), data.end(), pred);

    // Act
    ++it;

    // Assert
    EXPECT_EQ(*it, 4);
}

// Тест для FilterAdaptor
TEST(FilterTest, GetPredicateTest) {
    // Arrange
    auto pred = [](int val) { return val > 0; };
    ranges::view::FilterAdaptor<decltype(pred)> adaptor(pred);

    // Act
    auto retrievedPred = adaptor.GetPredicate();

    // Assert
    EXPECT_EQ(retrievedPred(-1), false);
    EXPECT_EQ(retrievedPred(1), true);
}

TEST(FilterTest, FilterEvenNumbersTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto pred = [](int val) { return val % 2 == 0; };
    auto filteredRange = data | ranges::view::filter(pred);

    // Act
    std::vector<int> filteredData;
    for (int i: filteredRange) {
        filteredData.push_back(i);
    }

    // Assert
    for (const auto& num: filteredData) {
        EXPECT_EQ(num % 2, 0);
    }
    EXPECT_EQ(filteredData.size(), 5);
}

TEST(FilterTest, FilterEvenNumbersTest2) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto pred = [](int val) { return val % 2 == 0; };
    auto filteredRange = data | ranges::view::filter(pred);

    // Act
    std::vector<int> filteredData(filteredRange.begin(), filteredRange.end());

    // Assert
    for (const auto& num: filteredData) {
        EXPECT_EQ(num % 2, 0);
    }
    EXPECT_EQ(filteredData.size(), 5);
}

TEST(TakeTest, TakeTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Act
    std::vector<int> filteredData;
    for (int i: data
                | ranges::view::take(3)
            ) {
        filteredData.push_back(i);
    }

    // Assert
    int j = 0;
    for (const auto& num: filteredData) {
        EXPECT_EQ(num, data[j++]);

    }
    EXPECT_EQ(filteredData.size(), 3);
}

TEST(TakeTest, TakeFilterTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Act
    std::vector<int> filteredData;
    for (int i: data
                | ranges::view::filter([](int x) { return x % 2 == 0; })
                | ranges::view::take(3)
            ) {
        filteredData.push_back(i);
    }

    // Assert
    for (const auto& num: filteredData) {
        EXPECT_EQ(num % 2, 0);

    }
    EXPECT_EQ(filteredData.size(), 3);
}

TEST(TakeTest, TakeTransformTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Act
    std::vector<int> filteredData;
    for (int i: data
                | ranges::view::transform([](int i) { return i * 2; })
                | ranges::view::take(3)
            ) {
        filteredData.push_back(i);
    }

    // Assert
    for (size_t i = 0; i < filteredData.size(); ++i) {
        EXPECT_EQ(filteredData[i], data[i] * 2);
    }
    EXPECT_EQ(filteredData.size(), 3);
}

TEST(ReverseTest, ReverseSimpleTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

    // Act
    std::vector<int> modify_data;
    for (int i: data
                | ranges::view::reverse()
            ) {
        modify_data.push_back(i);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}

TEST(ReverseTest, ReverseComplicatedTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result = {36, 16, 4};

    // Act
    std::vector<int> modify_data;
    for (int i: data
                | ranges::view::filter([](int x) { return x % 2 == 0; })
                | ranges::view::transform([](int i) { return i * i; })
                | ranges::view::take(3)
                | ranges::view::reverse()
            ) {
        modify_data.push_back(i);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}

TEST(DropTest, DropSimpleTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result = {6, 7, 8, 9, 10};

    // Act
    std::vector<int> modify_data;
    for (int i: data
                | ranges::view::drop(5)
            ) {
        modify_data.push_back(i);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}

TEST(DropTest, DropComplicatedTest) {
    // Arrange
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result = {16, 4};

    // Act
    std::vector<int> modify_data;
    for (int i: data
                | ranges::view::filter([](int x) { return x % 2 == 0; })
                | ranges::view::transform([](int i) { return i * i; })
                | ranges::view::take(3)
                | ranges::view::reverse()
                | ranges::view::drop(1)
            ) {
        modify_data.push_back(i);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}

TEST(KeyTest, KeySimpleTest) {
    // Arrange
    std::map<std::string, int> data = {{"apple",  5},
                                       {"banana", 3},
                                       {"cherry", 7}};
    std::vector<std::string> result = {"apple", "banana", "cherry"};

    // Act
    std::vector<std::string> modify_data;
    for (const auto& key: data
                          | ranges::view::keys()
            ) {
        modify_data.push_back(key);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}


TEST(KeyTest, KeyComplicatedTest) {
    // Arrange
    std::map<std::string, int> data = {{"apple",  5},
                                       {"banana", 3},
                                       {"cherry", 7}};
    std::vector<std::string> result = {"apple"};

    // Act
    std::vector<std::string> modify_data;
    for (const auto& key: data
                          | ranges::view::keys()
                          | ranges::view::take(2)
                          | ranges::view::reverse()
                          | ranges::view::drop(1)
            ) {
        modify_data.push_back(key);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}


TEST(KeyTest, KeyTransformFilterTest) {
    // Arrange
    std::map<std::string, int> data = {
            {"a apple",     5},
            {"a banana",    3},
            {"a cherry",    7},
            {"a nut",       7},
            {"a orange",    7},
            {"a apricot",   7},
            {"a avocado",   7},
            {"a pineapple", 7}
    };
    std::vector<std::string> result = {"avocado", "apricot", "apple"};

    // Act
    std::vector<std::string> modify_data;
    for (const auto& key: data
                          | ranges::view::keys()
                          | ranges::view::transform([](const std::string& s) { return s.substr(2); })
                          | ranges::view::take(5)
                          | ranges::view::reverse()
                          | ranges::view::drop(1)
                          | ranges::view::filter([](const std::string& x) { return x.starts_with("a"); })
            ) {
        modify_data.push_back(key);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}


TEST(ValuesTest, ValuesSimpleTest) {
    // Arrange
    std::map<std::string, int> data = {{"apple",  5},
                                       {"banana", 3},
                                       {"cherry", 7}};
    std::vector<int> result = {5, 3, 7};

    // Act
    std::vector<int> modify_data;
    for (const auto& key: data
                          | ranges::view::values()
            ) {
        modify_data.push_back(key);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}

TEST(ValuesTest, ValuesComplicatedTest) {
    // Arrange
    std::map<std::string, int> data = {
            {"a apple",     1},
            {"a banana",    2},
            {"a cherry",    3},
            {"a nut",       4},
            {"a orange",    5},
            {"a apricot",   6},
            {"a avocado",   7},
            {"a pineapple", 8}
    };
    std::vector<int> result = {16, 4, 36};

    // Act
    std::vector<int> modify_data;
    for (const auto& key: data
                          | ranges::view::values()
                          | ranges::view::transform([](int i) { return i * i; })
                          | ranges::view::filter([](int x) { return x % 2 == 0; })
                          | ranges::view::take(5)
                          | ranges::view::reverse()
                          | ranges::view::drop(1)
            ) {
        modify_data.push_back(key);
    }

    // Assert
    for (size_t i = 0; i < modify_data.size(); ++i) {
        EXPECT_EQ(modify_data[i], result[i]);
    }
}