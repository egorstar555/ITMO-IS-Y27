#include <lib/BinaryTreeSTL.h>

#include <gtest/gtest.h>
#include <sstream>

TEST(BinarySearchTreeTest, DefaultConstructor) {
    // Arrange
    BinarySearchTree<int> tree;

    // Act
    bool isEmpty = tree.empty();
    size_t size = tree.size();

    // Assert
    EXPECT_TRUE(isEmpty);
    EXPECT_EQ(size, 0);
}

TEST(BinarySearchTreeTest, CopyConstructorPre) {
    // Arrange
    BinarySearchTree<int, PreOrderTag> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Act
    BinarySearchTree<int, PreOrderTag> tree2(tree);
    bool isEqual = (tree == tree2);

    // Assert
    EXPECT_EQ(isEqual, true);
}

TEST(BinarySearchTreeTest, CopyConstructorIn) {
    // Arrange
    BinarySearchTree<int, InOrderTag> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Act
    BinarySearchTree<int, InOrderTag> tree2(tree);
    bool isEqual = (tree == tree2);

    // Assert
    EXPECT_EQ(isEqual, true);
}

TEST(BinarySearchTreeTest, CopyConstructorPost) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Act
    BinarySearchTree<int, PostOrderTag> tree2(tree);
    bool isEqual = (tree == tree2);

    // Assert
    EXPECT_EQ(isEqual, true);
}

TEST(BinarySearchTreeTest, InsertMethod) {
    // Arrange
    BinarySearchTree<int> tree;

    // Act
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    // Assert
    EXPECT_EQ(tree.size(), 3);
}

TEST(BinarySearchTreeTest, ConstructorFromRangeIterators) {
    // Arrange
    struct IntComparator {
        bool operator()(int a, int b) const {
            return a < b;
        }
    };
    std::vector<int> values = {5, 2, 8, 1, 4, 7, 9};

    std::vector<int> expected = {5, 2, 1, 4, 8, 7, 9};
    std::vector<int> result;

    // Act
    BinarySearchTree<int, PreOrderTag, IntComparator> bst(values.begin(), values.end(), IntComparator());
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, InsertFromRangeIterators) {
    // Arrange
    std::vector<int> values = {5, 2, 8, 1, 4, 7, 9};
    std::vector<int> expected = {5, 2, 1, 4, 8, 7, 9};
    std::vector<int> result;

    // Act
    BinarySearchTree<int> bst;
    bst.insert(values.begin(), values.end());
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ConstructorFromInitList) {
    // Arrange
    std::vector<int> expected = {5, 2, 1, 4, 8, 7, 9};
    std::vector<int> result;

    // Act
    BinarySearchTree<int> bst = {5, 2, 8, 1, 4, 7, 9};
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, InsertFromInitList) {
    // Arrange
    std::vector<int> expected = {5, 2, 1, 4, 8, 7, 9};
    std::vector<int> result;

    // Act
    BinarySearchTree<int> bst;
    bst.insert({5, 2, 8, 1, 4, 7, 9});
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}



TEST(BinarySearchTreeTest, MergeTest) {
    // Arrange
    BinarySearchTree<int> bst = {5, 2, 8, 1};
    BinarySearchTree<int> bst2 = {4, 7, 9, 3};
    std::vector<int> expected = {5, 2, 1, 4, 3, 8, 7, 9};
    std::vector<int> result;

    // Act
    bst.merge(bst2);
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert

    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ClearTest) {
    // Arrange
    BinarySearchTree<int> bst = {5, 2, 8, 1};

    // Act
    bst.clear();

    // Assert
    ASSERT_EQ(bst.size(), 0);
    ASSERT_TRUE(bst.empty());
}

TEST(BinarySearchTreeTest, EraseTest) {
    // Arrange
    BinarySearchTree<int> bst = {1, 1, 1, 1};

    // Act
    size_t n = bst.erase(1);

    // Assert
    ASSERT_EQ(bst.size(), 0);
    ASSERT_EQ(n, 4);
    ASSERT_TRUE(bst.empty());
}


TEST(BinarySearchTreeTest, ExtractTest) {
    // Arrange
    BinarySearchTree<int> bst  = {5, 2, 8, 1, 4, 7, 9, 3};
    std::vector<int> expected = {5, 3, 1, 4, 8, 7, 9};
    std::vector<int> result;

    // Act
    Node<int> *ext_node = bst.extract(2);
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
    ASSERT_EQ(ext_node->data, 2);
}

TEST(BinarySearchTreeTest, FindTest) {
    // Arrange
    BinarySearchTree<int> bst  = {5, 2, 8, 1, 4, 7, 9, 3};

    // Act
    auto it1 = bst.find(1);
    auto it2 = bst.find(10);

    // Assert
    ASSERT_EQ(*it1, 1);
    ASSERT_EQ(it2, bst.end());
}

TEST(BinarySearchTreeTest, CountTest) {
    // Arrange
    BinarySearchTree<int> bst  = {3, 5, 2, 8, 3, 1, 4, 7, 9, 3};

    // Act
    auto n1 = bst.count(3);
    auto n2 = bst.count(10);

    // Assert
    ASSERT_EQ(n1, 3);
    ASSERT_EQ(n2, 0);
}

TEST(BinarySearchTreeTest, ContainsTest) {
    // Arrange
    BinarySearchTree<int> bst  = {3, 5, 2, 8, 3, 1, 4, 7, 9, 3};

    // Act
    auto n1 = bst.contains(5);
    auto n2 = bst.contains(10);

    // Assert
    ASSERT_EQ(n1, true);
    ASSERT_EQ(n2, false);
}

TEST(BinarySearchTreeTest, LowerBoundFindTest) {
    // Arrange
    BinarySearchTree<int> bst  = {5, 2, 8, 1, 4, 7, 9, 3};

    // Act
    auto it1 = bst.lower_bound(6);
    auto it2 = bst.lower_bound(10);
    auto it3 = bst.lower_bound(5);

    // Assert
    ASSERT_EQ(*it1, 7);
    ASSERT_EQ(it2, bst.end());
    ASSERT_EQ(*it3, 5);
}

TEST(BinarySearchTreeTest, UpperBoundFindTest) {
    // Arrange
    BinarySearchTree<int> bst  = {5, 2, 8, 1, 4, 7, 9, 3};

    // Act
    auto it1 = bst.upper_bound(6);
    auto it2 = bst.upper_bound(10);
    auto it3 = bst.upper_bound(5);

    // Assert
    ASSERT_EQ(*it1, 7);
    ASSERT_EQ(it2, bst.end());
    ASSERT_EQ(*it3, 7);
}

TEST(BinarySearchTreeTest, EqualRangeTest) {
    // Arrange
    BinarySearchTree<int> bst  = {5, 2, 8, 1, 4, 7, 9, 3};

    // Act
    auto it1 = bst.equal_range(5);

    // Assert
    ASSERT_EQ(*(it1.first), 5);
    ASSERT_EQ(*(it1.second), 7);
}

TEST(BinarySearchTreeTest, EqualityOperator) {
    // Arrange
    BinarySearchTree<int> tree1;
    tree1.insert(5);
    tree1.insert(3);

    BinarySearchTree<int> tree2;
    tree2.insert(5);
    tree2.insert(3);

    // Assert
    EXPECT_EQ(tree1, tree2);
}

TEST(BinarySearchTreeTest, CopyAssignmentOperatorPre) {
    // Arrange
    BinarySearchTree<int> bst1;
    bst1.insert(5);
    bst1.insert(3);

    BinarySearchTree<int> bst2;

    // Act
    bst2 = bst1;

    // Assert
    ASSERT_EQ(bst1.size(), bst2.size());
    ASSERT_TRUE(std::equal(bst1.begin(), bst1.end(), bst2.begin()));
}

TEST(BinarySearchTreeTest, CopyAssignmentOperatorIn) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst1;
    bst1.insert(5);
    bst1.insert(3);

    BinarySearchTree<int, InOrderTag> bst2;

    // Act
    bst2 = bst1;

    // Assert
    ASSERT_EQ(bst1.size(), bst2.size());
    ASSERT_TRUE(std::equal(bst1.begin(), bst1.end(), bst2.begin()));
}

TEST(BinarySearchTreeTest, CopyAssignmentOperatorPost) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst1;
    bst1.insert(5);
    bst1.insert(3);

    BinarySearchTree<int, PostOrderTag> bst2;

    // Act
    bst2 = bst1;

    // Assert
    ASSERT_EQ(bst1.size(), bst2.size());
    ASSERT_TRUE(std::equal(bst1.begin(), bst1.end(), bst2.begin()));
}

TEST(BinarySearchTreeTest, PreOrderTraversal) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    std::vector<int> expected = {5, 3, 7};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreeTraversal) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {10, 5, 3, 7, 15, 20};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, PostOrderTraversal) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    std::vector<int> expected = {3, 7, 5};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreePostOrderTraversal) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {3, 7, 5, 20, 15, 10};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, InOrderTraversal) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    std::vector<int> expected = {3, 5, 7};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreeInOrderTraversal) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {3, 5, 7, 10, 15, 20};
    std::vector<int> result;

    // Act
    for (auto it = bst.begin(); it != bst.end(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreeTraversalConst) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {10, 5, 3, 7, 15, 20};
    std::vector<int> result;

    // Act
    for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreePostOrderTraversalConst) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {3, 7, 5, 20, 15, 10};
    std::vector<int> result;

    // Act
    for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexTreeInOrderTraversalConst) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {3, 5, 7, 10, 15, 20};
    std::vector<int> result;

    // Act
    for (auto it = bst.cbegin(); it != bst.cend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ReverseIteratorTraversal) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);

    std::vector<int> expected = {7, 3, 5};
    std::vector<int> result;

    // Act
    for (auto it = bst.rbegin(); it != bst.rend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexPreOrderReverseIteratorTraversal) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {20, 15, 7, 3, 5, 10};
    std::vector<int> result;

    // Act
    for (auto it = bst.rbegin(); it != bst.rend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, InOrderReverseIteratorTraversal) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {20, 15, 10, 7, 5, 3};
    std::vector<int> result;

    // Act
    for (auto it = bst.rbegin(); it != bst.rend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexPostOrderReverseIteratorTraversal) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {10, 15, 20, 5, 7, 3};
    std::vector<int> result;

    // Act
    for (auto it = bst.rbegin(); it != bst.rend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexPreOrderReverseIteratorTraversalConst) {
    // Arrange
    BinarySearchTree<int> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {20, 15, 7, 3, 5, 10};
    std::vector<int> result;

    // Act
    for (auto it = bst.crbegin(); it != bst.crend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, InOrderReverseIteratorTraversalConst) {
    // Arrange
    BinarySearchTree<int, InOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {20, 15, 10, 7, 5, 3};
    std::vector<int> result;

    // Act
    for (auto it = bst.crbegin(); it != bst.crend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ComplexPostOrderReverseIteratorTraversalConst) {
    // Arrange
    BinarySearchTree<int, PostOrderTag> bst;
    bst.insert(10);
    bst.insert(5);
    bst.insert(3);
    bst.insert(7);
    bst.insert(15);
    bst.insert(20);

    std::vector<int> expected = {10, 15, 20, 5, 7, 3};
    std::vector<int> result;

    // Act
    for (auto it = bst.crbegin(); it != bst.crend(); ++it) {
        result.push_back(*it);
    }

    // Assert
    ASSERT_EQ(expected, result);
}

TEST(BinarySearchTreeTest, ConstructorWithAllocator) {
    // Arrange
    using IntTree = BinarySearchTree<int>;
    using IntAllocator = std::allocator<int>;
    IntAllocator allocator;

    // Act
    IntTree tree(allocator);

    // Assert
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(tree.get_allocator(), allocator);
}

TEST(BinarySearchTreeTest, CopyConstructorWithAllocator) {
    // Arrange
    using IntTree = BinarySearchTree<int>;
    using IntAllocator = std::allocator<int>;
    IntAllocator allocator;
    IntTree originalTree(allocator);
    originalTree.insert(10);

    // Act
    IntTree treeCopy(originalTree, allocator);

    // Assert
    EXPECT_EQ(treeCopy.size(), 1);
    EXPECT_EQ(*(treeCopy.begin()), 10);
    EXPECT_EQ(treeCopy.get_allocator(), allocator);
}

TEST(BinarySearchTreeTest, SwapTest) {
    // Arrange
    using IntTree = BinarySearchTree<int>;
    using IntAllocator = std::allocator<int>;
    IntAllocator allocator;
    IntTree tree1(allocator);
    IntTree tree2(allocator);

    tree1.insert(10);
    tree1.insert(20);
    tree2.insert(30);
    tree2.insert(40);

    // Act
    tree1.swap(tree2);

    // Assert
    EXPECT_EQ(tree1.size(), 2);
    EXPECT_EQ(tree2.size(), 2);
    EXPECT_EQ(*(tree1.begin()), 30);
    EXPECT_EQ(*(tree2.begin()), 10);
}

TEST(BinarySearchTreeTest, SwapTest2) {
    // Arrange
    using IntTree = BinarySearchTree<int>;
    using IntAllocator = std::allocator<int>;
    IntAllocator allocator;
    IntTree tree1(allocator);
    IntTree tree2(allocator);
    IntTree tree3(allocator);

    tree1.insert(10);
    tree1.insert(20);
    tree1.insert(50);
    tree2.insert(30);
    tree2.insert(40);

    // Act
    tree3.swap(tree1, tree2);

    // Assert
    EXPECT_EQ(tree1.size(), 2);
    EXPECT_EQ(tree2.size(), 3);
    EXPECT_EQ(*(tree1.begin()), 30);
    EXPECT_EQ(*(tree2.begin()), 10);
}
