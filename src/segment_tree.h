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

        // Caso 1: No hay interseccion
        if (query_left > query_right || curr_left > curr_right) {
            return 0;
        }

        // Caso 2: Interseccion total

        if (query_left == curr_left && query_right == curr_right) {
            return current->sum;
        } // como la interseccion total, se devuelve la suma ya precalculada


        // Caso 3: Interseccion parcial
        int mid = (curr_left + curr_right) / 2;
        return _query(current->left, query_left, std::min(query_right, mid), curr_left, mid) +
               _query(current->right, std::max(query_left, mid+1), query_right, mid+1, curr_right);

        // En este caso, se divide el intervalo y se llama recursivamente a las sub-ramas
    }

    void _update(TNode* node, int left, int right, int index, const T &value) {
        if (left == right) { // Es una hoja, caso base
            node->sum = value;
            node->min_val = value;
            node->max_val = value;
            return;
        }

        // Caso recursivo -> tiene que buscar el nodo hoja a actualizar
        int mid = (left + right) / 2;

        if (index <= mid) // modificar rama izquierda
            _update(node->left, left, mid, index, value);
        else // modificar rama derecha
            _update(node->right, mid + 1, right, index, value);

        node->update(); // Actualizar despues de la actualizacion (backpropagation)
    }

    void deleteTree(TNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    T _query_max(TNode* node, int query_left, int query_right, int curr_left, int curr_right) {
        // Caso 1: No hay interseccion

        if (query_left > curr_right || query_right < curr_left) {
            throw std::invalid_argument("Query range is out of bounds");
        }

        // Caso 2: Interseccion total -> se devuelve el maximo ya precalculado
        if (query_left <= curr_left && query_right >= curr_right) {
            return node->max_val;
        }

        // Caso 3: Interseccion parcial
        int mid = (curr_left + curr_right) / 2;

        // Se divide el intervalo y se llama recursivamente a las sub-ramas
        return std::max(_query_max(node->left, query_left, query_right, curr_left, mid),
                        _query_max(node->right, query_left, query_right, mid + 1, curr_right));
    }

    T _query_min(TNode* node, int query_left, int query_right, int curr_left, int curr_right) {

        // Caso 1: No hay interseccion
        if (query_left > curr_right || query_right < curr_left) {
            throw std::invalid_argument("Query range is out of bounds");
        }

        // Caso 2: Interseccion total -> se devuelve el minimo ya precalculado
        if (query_left <= curr_left && query_right >= curr_right) {
            return node->min_val;
        }


        // Caso 3: Interseccion parcial
        int mid = (curr_left + curr_right) / 2;

        return std::min(_query_min(node->left, query_left, query_right, curr_left, mid),
                        _query_min(node->right, query_left, query_right, mid + 1, curr_right));
    }



public:
    SegmentTree() : root(nullptr), size(0) {};

    ~SegmentTree() {
        deleteTree(root);
    }

    explicit SegmentTree(TNode &n, int size) : root(n), size(size) {};

    template<int SZ>
    explicit SegmentTree(T (&arr)[SZ]) : size(SZ) {
        root = build(arr, 0, SZ - 1);
    };

    static TNode* build(T arr[], int left, int right) {
        if (left == right) return new TNode(arr[left]); // Nodo hoja
        int mid = (left + right) / 2; // Divide el array recursivamente
        TNode* left_child = build(arr, left, mid);
        TNode* right_child = build(arr, mid+1, right);
        return new TNode(left_child, right_child); // Nodo interno
    }

    [[nodiscard]] bool is_empty() const {
        return root == nullptr || size == 0;
    }

    /**
     *
     * @param left Interval start
     * @param right Interval end
     * @return Sum of intervals, or empty.
     */


    T query(int left, int right) {
        if (left < 0 || right >= size || left > right) {  // Added left > right check
            throw std::invalid_argument("Invalid query range");
        }
        return _query(root, left, right, 0, size-1);
    }

    void update(int index, const T &value) {
        if (index < 0 || index >= size)
            throw std::invalid_argument("Invalid update index");

        _update(root, 0, size - 1, index, value);
    }

    std::optional<T> query_max(int left, int right) {
        if (is_empty()) return std::optional<T>();

        return _query_max(root, left, right, 0, size - 1);

    }

    std::optional<T> query_min(int left, int right) {
        if (is_empty()) return std::optional<T>();

        return _query_min(root, left, right, 0, size - 1);
    }
};


#endif //SEGMENT_TREE_H
