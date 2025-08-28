#pragma once

struct PreOrderTag {};
struct PostOrderTag {};
struct InOrderTag {};

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
    Node* parent;
    explicit Node(const T& value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
};