#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <unordered_map>
#include "lib/AdaptersSTL.h"
#include "lib/IteratorRange.h"
#include "lib/transform.h"
#include "lib/filter.h"
#include "lib/take.h"
#include "lib/reverse.h"
#include "lib/drop.h"
#include "lib/keys.h"
#include "lib/values.h"

int main() {
    std::queue<int> v;

    for (int i: v
                | ranges::view::transform([](int i) { return i * i; })
                | ranges::view::filter([](int x) { return x % 2 == 0; })
                | ranges::view::take(3)
                | ranges::view::reverse()
                | ranges::view::drop(1)
            ) {
        std::cout << i << " ";
    }

//    std::unordered_map<std::string, int> myMap = {
    std::map<std::string, int> myMap = {
            {"a apple",     1},
            {"a banana",    2},
            {"a cherry",    3},
            {"a nut",       4},
            {"a orange",    5},
            {"a apricot",   6},
            {"a avocado",   7},
            {"a pineapple", 8}
    };

    for (const auto &key: myMap
                          | ranges::view::keys()
                          | ranges::view::transform([](const std::string &s) { return s.substr(2); })
                          | ranges::view::take(5)
                          | ranges::view::reverse()
                          | ranges::view::drop(1)
                          | ranges::view::filter([](const std::string &x) { return x.starts_with("a"); })
            ) {
        std::cout << key << "\n";
    }
//
//    for (const auto &key: myMap
//                          | ranges::view::values()
//                          | ranges::view::transform([](int i) { return i * i; })
//                          | ranges::view::filter([](int x) { return x % 2 == 0; })
//                          | ranges::view::take(5)
//                          | ranges::view::reverse()
//                          | ranges::view::drop(1)
//            ) {
//        std::cout << key << "\n";
//    }


    return 0;
}
