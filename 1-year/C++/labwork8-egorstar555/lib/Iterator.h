#pragma once

#include "Node.h"
#include <iostream>

template<typename T, typename TraverseType = PreOrderTag, bool IsConst = false>
class Iterator {
public:
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using iterator_category = std::bidirectional_iterator_tag;

    using node_ptr = std::conditional_t<IsConst, const Node<T>*, Node<T>*>;

    template<bool WasConst, class = std::enable_if_t<IsConst || !WasConst>>
    Iterator(const Iterator<T, TraverseType, WasConst>& other)
            : tree_traversal_(other.tree_traversal_), root_(other.root_), current_(other.current_) {}

    explicit Iterator(node_ptr root, node_ptr current, TraverseType tree_traversal)
            : root_(root),
              current_(current),
              tree_traversal_(tree_traversal) {
        SetCurrent(tree_traversal);
    }

    reference operator*() const {
        return current_->data;
    }

    pointer operator->() const {
        return &(current_->data);
    }

    Iterator& operator++() {
        DoublePlus(tree_traversal_);

        return *this;
    }

    Iterator operator++(int) {
        Iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    Iterator& operator--() {
        DoubleMinus(tree_traversal_);

        return *this;
    }

    Iterator operator--(int) {
        Iterator tmp(*this);
        --*this;
        return tmp;
    }

    bool operator==(const Iterator& other) const {
        return current_ == other.current_;
    }

    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

    node_ptr GetCurrent() {
        return current_;
    }

private:
    node_ptr current_;
    node_ptr root_;
    TraverseType tree_traversal_;

    node_ptr FindPreOrderMaxNode(node_ptr node) {
        if (node) {
            do {
                while (node->right) {
                    node = node->right;
                }
                if (node->left) {
                    node = node->left;
                }
            } while (node->right);
        }

        return node;
    }

    node_ptr FindPostOrderMinNode(node_ptr node) {
        if (node) {
            do {
                while (node->left) {
                    node = node->left;
                }
                if (node->right) {
                    node = node->right;
                }
            } while (node->right || node->left);
        }

        return node;
    }

    node_ptr FindInOrderLeftNode(node_ptr node) {
        if (node) {
            while (node->left) {
                node = node->left;
            }
        }

        return node;
    }

    node_ptr FindInOrderRightNode(node_ptr node) {
        if (node) {
            while (node->right) {
                node = node->right;
            }
        }

        return node;
    }

    void SetCurrent(PreOrderTag) {}

    void SetCurrent(PostOrderTag) {
        current_ = FindPostOrderMinNode(current_);
    }

    void SetCurrent(InOrderTag) {
        current_ = FindInOrderLeftNode(current_);
    }

    void DoublePlus(PreOrderTag) {
        if (!current_) {
            current_ = root_;

            return;
        }
        if (current_->left) {
            current_ = current_->left;

            return;
        }
        if (current_->right) {
            current_ = current_->right;

            return;
        }

        node_ptr parent = current_->parent;
        while (parent) {
            if (current_ == parent->left && parent->right) {
                current_ = parent->right;

                return;
            }
            current_ = parent;
            parent = parent->parent;

        }
        current_ = nullptr;
    }

    void DoublePlus(PostOrderTag) {
        if (!current_) {
            current_ = root_;
            current_ = FindPostOrderMinNode(current_);

            return;
        }
        node_ptr parent = current_->parent;
        if (!parent) {
            current_ = nullptr;

            return;
        }
        if (current_ == parent->right) {
            current_ = parent;

            return;
        }
        if (parent->right) {
            current_ = parent->right;
            current_ = FindPostOrderMinNode(current_);
        } else {
            current_ = parent;
        }
    }

    void DoublePlus(InOrderTag) {
        if (!current_) {
            current_ = root_;
            current_ = FindInOrderLeftNode(current_);

            return;
        }
        if (current_->right) {
            current_ = current_->right;
            current_ = FindInOrderLeftNode(current_);

            return;
        }
        node_ptr parent = current_->parent;
        if (current_ == current_->left) {
            current_ = current_->left;
        } else {
            while (parent && current_ == parent->right) {
                current_ = parent;
                parent = parent->parent;
            }
            if (parent && current_ == parent->left) {
                current_ = parent;
            } else {
                current_ = nullptr;
            }
        }
    }

    void DoubleMinus(PreOrderTag) {
        if (!current_) {
            current_ = root_;
            current_ = FindPreOrderMaxNode(current_);

            return;
        }

        node_ptr parent = current_->parent;
        if (parent == nullptr) {
            current_ = nullptr;

            return;
        }

        if (current_ == parent->left) {
            current_ = parent;

            return;
        }

        if (parent->left) {
            current_ = parent->left;
            current_ = FindPreOrderMaxNode(current_);
        } else {
            current_ = parent;
        }
    }

    void DoubleMinus(PostOrderTag) {
        if (!current_) {
            current_ = root_;

            return;
        }
        if (current_->right) {
            current_ = current_->right;

            return;
        }
        if (current_->left) {
            current_ = current_->left;

            return;
        }
        node_ptr parent = current_->parent;
        while (parent) {
            if (parent->left) {
                current_ = parent->left;

                return;
            }
            current_ = parent;
            parent = parent->parent;
        }
        current_ = nullptr;

    }

    void DoubleMinus(InOrderTag) {
        if (!current_) {
            current_ = root_;
            current_ = FindInOrderRightNode(current_);

            return;
        }
        if (current_->left) {
            current_ = current_->left;
            current_ = FindInOrderRightNode(current_);

            return;
        }
        node_ptr parent = current_->parent;
        while (parent && current_ == parent->left) {
            current_ = parent;
            parent = parent->parent;
        }
        if (current_ == parent->right) {
            current_ = parent;
        } else {
            current_ = nullptr;
        }
    }
};
