//
// Created by Salva on 31/05/25.
//

#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H
#include <algorithm>
#include <optional>
#include <cmath>
#include <stdexcept>

#include "node.h"

/**
 *
 *      Unit Segment-Tree Implementation.
 *
 * Specs:
 * - Using nodes object-oriented for persistence
 * - Update without rebuilding all the tree
 * @tparam T Type of nodes who supports arithmetic operations & comparisons
 */
template<typename T>
class SegmentTree {
    using TNode = Node<T>;

    TNode *root;
    int size;

    /**
     *
     * @param current Current node index
     * @param query_left Query interval left
     * @param query_right Query interval right
     * @param curr_left Tree interval left
     * @param curr_right Tree interval right
     * @return Sum of all values inside interval
     */
    T _query(TNode *current,
             int query_left, int query_right,
             int curr_left, int curr_right) {
        if (curr_left > curr_right) {
            return 0;
        }

        if (query_left == curr_left && query_right == curr_right) {
            return current->sum;
        }

        int mid = (curr_left + curr_right) / 2;
        int disjoint_right = std::min(query_right, curr_right);
        int disjoint_left = std::max(query_left, curr_left);
        return _query(current->left, query_left, mid, curr_left, disjoint_right)
             + _query(current->right, mid + 1, curr_right, disjoint_left, query_right);
    }

    void _update(TNode* node, int left, int right, int index, T &value) {
        if (left == right) { // Interval found
            node->sum = value;
            return;
        }

        int mid = (left + right) / 2;
        if (mid > index)
            _update(node->left, left, mid, index, value);
        else
            _update(node->right, mid + 1, right, index, value);

        // After writing new value, update each node at the stack
        node->update();
    }

public:
    SegmentTree() : root(nullptr), size(0) {};

    explicit SegmentTree(TNode &n, int size) : root(n), size(size) {};

    template<int SZ>
    explicit SegmentTree(T (&arr)[SZ]) : size(SZ) {
        root = build(arr, 0, SZ - 1);
    };

    static TNode *build(T arr[], int left, int right) {
        if (left == right) return new TNode(arr[left]); // arr[left] == arr[right]

        int mid = (left + right) / 2;
        return new TNode(build(arr, left, mid), build(arr, mid + 1, right));
    }

    bool is_empty() {
        return root == nullptr;
    }

    /**
     *
     * @param left Interval start
     * @param right Interval end
     * @return Sum of intervals, or empty.
     */
    T query(int left, int right) {
        if (is_empty()) return T{};

        return left > right ? 0 : _query(root, left, right, 0, size - 1);
    }

    void update(int index, T &value) {
        if (index < 0 || index >= size) {
            throw std::invalid_argument("Invalid index to update: " + std::to_string(index));
        }

        if (is_empty()) return;

        _update(root, 0, size - 1, index, value);
    }

    std::optional<T> query_max() {
        if (is_empty()) return std::optional<T>();
        // TODO
        return std::optional<T>();
    }

    std::optional<T> query_min() {
        if (is_empty()) return std::optional<T>();
        // TODO
        return std::optional<T>();
    }
};


#endif //SEGMENT_TREE_H
