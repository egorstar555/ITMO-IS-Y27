#pragma once

#include <memory>
#include <functional>

#include "Iterator.h"
#include "Node.h"

template<typename T, typename TraverseType = PreOrderTag, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
class BinarySearchTree {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = Iterator<T, TraverseType>;
    using const_iterator = Iterator<T, TraverseType, true>;
    using difference_type = std::ptrdiff_t;
    using size_type = size_t;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using allocator_type = Allocator;

    using allocaton_node = typename std::allocator_traits<allocator_type>::template rebind_alloc<Node<T>>;
    using traits_node = std::allocator_traits<allocaton_node>;

    using node_ptr = Node<T>*;

    BinarySearchTree() : root_(nullptr) {}

    BinarySearchTree(const BinarySearchTree<value_type, TraverseType, Compare, allocator_type>& copy)
            : root_(nullptr),
              comparator_(copy.comparator_),
              allocator_(copy.allocator_),
              alloc_node_(copy.alloc_node_),
              tree_traversal_(copy.tree_traversal_),
              size_(copy.size_) {
        if (copy.root_) {
            root_ = TreeCopy_(copy.root_);
        }
    }

    explicit BinarySearchTree(allocator_type allocator)
            : root_(nullptr),
              allocator_(allocator),
              alloc_node_(allocator) {}

    BinarySearchTree(const BinarySearchTree<value_type, TraverseType, Compare, allocator_type>& copy,
                     allocator_type allocator)
            : root_(nullptr),
              comparator_(copy.comparator_),
              allocator_(allocator),
              alloc_node_(allocator),
              tree_traversal_(copy.tree_traversal_),
              size_(copy.size_) {
        if (copy.root_) {
            root_ = TreeCopy_(copy.root_);
        }
    }

    template<typename InputIt>
    BinarySearchTree(InputIt first, InputIt last) : root_(nullptr) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }

    template<typename InputIt>
    BinarySearchTree(InputIt first, InputIt last, const Compare& comp) : root_(nullptr), comparator_(comp) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }

    BinarySearchTree(std::initializer_list<value_type> init_list)
            : root_(nullptr) {
        for (const auto& item: init_list) {
            insert(item);
        }
    }

    ~BinarySearchTree() {
        Clear_(root_);
    }

    BinarySearchTree& operator=(const BinarySearchTree<value_type, TraverseType, Compare, allocator_type>& copy) {
        if (this != &copy) {
            Clear_(root_);
            comparator_ = copy.comparator_;
            allocator_ = copy.allocator_;
            alloc_node_ = copy.alloc_node_;
            tree_traversal_ = copy.tree_traversal_;
            size_ = copy.size_;
            if (copy.root_) {
                root_ = TreeCopy_(copy.root_);
            }
        }

        return *this;
    }

    iterator begin() {
        return iterator(root_, root_, tree_traversal_);
    }

    iterator end() {
        return iterator(root_, nullptr, tree_traversal_);
    }

    const_iterator cbegin() const {
        return const_iterator(root_, root_, tree_traversal_);
    }

    const_iterator cend() const {
        return const_iterator(root_, nullptr, tree_traversal_);
    }

    bool operator==(const BinarySearchTree<T, TraverseType, Compare, Allocator>& other) const {
        return std::equal(this->cbegin(), this->cend(),
                          other.cbegin(), other.cend());
    }

    bool operator!=(const BinarySearchTree<T, TraverseType, Compare, Allocator>& other) const {
        return !(*this == other);
    }

    void swap(BinarySearchTree& other) {
        std::swap(root_, other.root_);
        std::swap(comparator_, other.comparator_);
        std::swap(allocator_, other.allocator_);
        std::swap(alloc_node_, other.alloc_node_);
        std::swap(tree_traversal_, other.tree_traversal_);
        std::swap(size_, other.size_);
    }

    void swap(BinarySearchTree& lhs, BinarySearchTree& rhs) {
        std::swap(lhs.root_, rhs.root_);
        std::swap(lhs.comparator_, rhs.comparator_);
        std::swap(lhs.allocator_, rhs.allocator_);
        std::swap(lhs.alloc_node_, rhs.alloc_node_);
        std::swap(lhs.tree_traversal_, rhs.tree_traversal_);
        std::swap(lhs.size_, rhs.size_);
    }

    size_type size() const {
        return size_;
    }

    size_type max_size() {
        return traits_node::max_size(alloc_node_) / sizeof(Node<T>);
    }

    bool empty() {
        return root_ == nullptr;
    }

    reverse_iterator rbegin() {
        return std::reverse_iterator(end());
    }

    reverse_iterator rend() {
        return std::reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return std::reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return std::reverse_iterator(cbegin());
    }

    allocator_type get_allocator() { return allocator_; }

    void insert(const_reference value) {
        root_ = Insert_(root_, value);
    }

    template<typename InputIt>
    void insert(InputIt first, InputIt last) {
        for (auto it = first; it != last; ++it) {
            insert(*it);
        }
    }

    void insert(std::initializer_list<value_type> init_list) {
        for (const auto& item: init_list) {
            insert(item);
        }
    }

    node_ptr extract(const_reference key) {
        node_ptr current_node = root_;
        node_ptr parent_node = nullptr;

        while (current_node && current_node->data != key) {
            parent_node = current_node;
            if (comparator_(key, current_node->data)) {
                current_node = current_node->left;
            } else {
                current_node = current_node->right;
            }
        }

        if (!current_node) {
            return node_ptr(nullptr);
        }

        return extract(current_node);
    }

    node_ptr extract(node_ptr q) {
        if (q->left == nullptr) {
            Transplant_(q, q->right);
        } else if (q->right == nullptr) {
            Transplant_(q, q->left);
        } else {
            node_ptr y = Minimum_(q->right);
            if (y->parent != q) {
                Transplant_(y, y->right);
                y->right = q->right;
                y->right->parent = y;
            }
            Transplant_(q, y);
            y->left = q->left;
            y->left->parent = y;
        }
        --size_;

        return q;
    }

    size_type erase(const_reference key) {
        size_type n = 0;
        while (extract(key) != nullptr) {
            ++n;
        }

        return n;
    }

    void merge(BinarySearchTree<value_type, TraverseType, Compare, Allocator>& a2) {
        MergeRecursive_(root_, a2.root_);
    }

    void clear() {
        Clear_(root_);
        size_ = 0;
        root_ = nullptr;
    }

    iterator find(const value_type& key) {
        node_ptr current = root_;
        while (current != nullptr) {
            if (comparator_(key, current->data)) {
                current = current->left;
            } else if (comparator_(current->data, key)) {
                current = current->right;
            } else {

                return iterator(root_, current, tree_traversal_);
            }
        }

        return end();
    }

    size_type count(const value_type& key) const {
        size_type count = 0;
        node_ptr current = root_;

        while (current != nullptr) {
            if (comparator_(key, current->data)) {
                current = current->left;
            } else if (comparator_(current->data, key)) {
                current = current->right;
            } else {
                ++count;
                current = current->right;
            }
        }

        return count;
    }

    bool contains(const value_type& key) {
        return find(key) != end();
    }

    iterator lower_bound(const T& key) {
        node_ptr current = root_;
        node_ptr lower_bound_node = nullptr;

        while (current) {
            if (!comparator_(current->data, key)) {
                lower_bound_node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (lower_bound_node) {

            return iterator(root_, lower_bound_node, tree_traversal_);
        } else {

            return end();
        }
    }

    iterator upper_bound(const T& key) {
        node_ptr current = root_;
        node_ptr upper_bound_node = nullptr;

        while (current) {
            if (comparator_(key, current->data)) {
                upper_bound_node = current;
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (upper_bound_node) {

            return iterator(root_, upper_bound_node, tree_traversal_);
        } else {

            return end();
        }
    }

    std::pair<iterator, iterator> equal_range(const T& key) {
        return std::make_pair(lower_bound(key), upper_bound(key));
    }


private:
    node_ptr root_;
    Compare comparator_;
    allocator_type allocator_;
    allocaton_node alloc_node_;
    TraverseType tree_traversal_;
    size_type size_{0};

    node_ptr Insert_(node_ptr node, const T& value) {
        if (node == nullptr) {
            node = traits_node::allocate(alloc_node_, 1);
            traits_node::construct(alloc_node_, node, value);
            ++size_;
            return node;
        }

        if (comparator_(value, node->data)) {
            node->left = Insert_(node->left, value);
            if (!node->left->parent) {
                node->left->parent = node;
            }
        } else {
            node->right = Insert_(node->right, value);
            if (!node->right->parent) {
                node->right->parent = node;
            }
        }

        return node;
    }

    void Clear_(node_ptr node) {
        if (node != nullptr) {
            Clear_(node->left);
            Clear_(node->right);
            traits_node::destroy(alloc_node_, node);
            traits_node::deallocate(alloc_node_, node, 1);
        }
    }

    node_ptr TreeCopy_(const node_ptr node) {
        if (!node) {
            return nullptr;
        }
        node_ptr new_node = traits_node::allocate(alloc_node_, 1);
        traits_node::construct(alloc_node_, new_node, node->data);

        new_node->left = TreeCopy_(node->left);
        new_node->right = TreeCopy_(node->right);

        if (new_node->left) {
            new_node->left->parent = new_node;
        }
        if (new_node->right) {
            new_node->right->parent = new_node;
        }

        return new_node;
    }

    void Transplant_(node_ptr u, node_ptr v) {
        if (!u->parent) {
            root_ = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        if (v) {
            v->parent = u->parent;
        }
    }

    node_ptr Minimum_(node_ptr x) {
        while (x->left) {
            x = x->left;
        }
        return x;
    }

    void MergeRecursive_(node_ptr& node1, node_ptr node2) {
        if (node2 == nullptr) {
            return;
        }

        insert(node2->data);

        MergeRecursive_(node1, node2->left);
        MergeRecursive_(node1, node2->right);
    }
};