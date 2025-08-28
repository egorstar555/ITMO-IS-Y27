#include <iostream>
#include "lib/BinaryTreeSTL.h"

int main() {
    BinarySearchTree<int> tree;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);

    for (auto i{tree.begin()}; i != tree.end(); i++) {
        std::cout << *i << "\n";
    }

    return 0;
}