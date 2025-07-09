#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H
#include <algorithm>
#include <optional>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#include "raylib.h"
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
     * Recursive helper for sum queries.
     * @param current curr node
     * @param query_left query left bound
     * @param query_right query right bound
     * @param curr_left curr left boundary
     * @param curr_right curr right boundary
     * @return SUMA TOTAL DEL QUERY LEFT Y QUERY RIGHT
     */
    T _query_sum(TNode *current,
             int query_left, int query_right,
             int curr_left, int curr_right,
             std::unordered_set<TNode*> &visited) {

        // Caso 1: No hay interseccion
        if (query_left > query_right || curr_left > curr_right || query_left > curr_right || query_right < curr_left) {
            return 0; 
        }

        visited.insert(current);

        // Caso 2: Interseccion total
        if (query_left <= curr_left && query_right >= curr_right) {
            return current->sum; // como la interseccion total, se devuelve la suma ya precalculada
        }

        // Caso 3: Interseccion total
        int mid = (curr_left + curr_right) / 2;
        return _query_sum(current->left, query_left, std::min(query_right, mid), curr_left, mid, visited) +
               _query_sum(current->right, std::max(query_left, mid+1), query_right, mid+1, curr_right, visited);
    }

    /**
     * funcion helper para actualizar un valor de forma recursiva
     * @param node curr nide
     * @param index indef to update
     * @param value NEW VAL
     */
    TNode* _update(TNode* node, int index, const T &value, std::unordered_set<TNode*> &visited) {
        if (!node || index < node->startRange || index > node->endRange) return nullptr;

        visited.insert(node);

        if (node->startRange == node->endRange) { // nodo hoja
            node->sum = value;
            node->min = value;
            node->max = value;
            return node;
        }

        auto r = _update(node->left, index, value, visited);
        if (!r) r = _update(node->right, index, value, visited);

        node->update(); // hacer backpropagation
        return r;
    }

    /**
     * @param node The current node to delete
     */
    void deleteTree(TNode* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    /**
     * helper para MAX queries
     * @param node curr
     * @param query_left QUERY left boundary
     * @param query_right QUERY right boundary
     * @param curr_left curr left boundary
     * @param curr_right curr right boundary
     * @return max value 
     */
    T _query_max(TNode* node, int query_left, int query_right, int curr_left, int curr_right, std::unordered_set<TNode*> &visited) {
        // Case 1: No intersection
        if (!node || query_left > curr_right || query_right < curr_left) {
            return std::numeric_limits<T>::min(); // Return smallest possible value
        }
        visited.insert(node);

        // Case 2: Total intersection - current node's range is fully within query range
        if (query_left <= curr_left && query_right >= curr_right) {
            return node->max; // Return pre-calculated max
        }

        // Case 3: Partial intersection - recurse into children
        int mid = (curr_left + curr_right) / 2;
        return std::max(_query_max(node->left, query_left, query_right, curr_left, mid, visited),
                        _query_max(node->right, query_left, query_right, mid + 1, curr_right, visited));
    }

    /**
     * helper recursivo para MIN queries
     * @param node  curr
     * @param query_left QUERY left boundary
     * @param query_right QUERY right boundary
     * @param curr_left curr left boundary
     * @param curr_right curr right boundary
     * @return min value
     */
    T _query_min(TNode* node, int query_left, int query_right, int curr_left, int curr_right, std::unordered_set<TNode*> &visited) {
        // Case 1: No intersection
        if (!node || query_left > curr_right || query_right < curr_left) {
            return std::numeric_limits<T>::max(); // Return largest possible value
        }
        visited.insert(node);

        // Case 2: Total intersection - current node's range is fully within query range
        if (query_left <= curr_left && query_right >= curr_right) {
            return node->min; // Return pre-calculated min
        }

        // Case 3: Partial intersection - recurse into children
        int mid = (curr_left + curr_right) / 2;
        return std::min(_query_min(node->left, query_left, query_right, curr_left, mid, visited),
                        _query_min(node->right, query_left, query_right, mid + 1, curr_right, visited));
    }

    // obtener depth recursivamente
    int _getDepth(TNode* node) {
        if (!node) {
            return 0;
        }
        return 1 + std::max(_getDepth(node->left), _getDepth(node->right));
    }


    void _drawNode(TNode* node, int x, int y, int h_spacing, int current_level, int max_draw_level, int tree_depth, int screenWidth, std::unordered_set<TNode*> &nodes) {
        if (!node || current_level > max_draw_level) return;

        // Calcula radio adaptativo (más pequeño a mayor profundidad)
        int base_radius = 40;
        int radius = std::max(20, base_radius - current_level * 4);

        // Textos
        std::string range_text = "[" + std::to_string(node->startRange) + ":" + std::to_string(node->endRange) + "]";
        std::string sum_text = std::to_string(node->sum);
        std::string min_text = "[" + std::to_string(node->min) + ", " + std::to_string(node->max) + "]";

        int fontSize = 20;

        // Centra los textos horizontalmente dentro del círculo
        int rangeWidth = MeasureText(range_text.c_str(), fontSize);
        int sumWidth = MeasureText(sum_text.c_str(), fontSize);
        int minWidth = MeasureText(min_text.c_str(), fontSize);

        if (nodes.count(node))
        {
            DrawCircle(x, y + 30, radius * 1.12, RED);
            DrawCircle(x, y + 30, radius, WHITE);
        }
        DrawCircleLines(x, y + 30, radius, BLACK);

        DrawText(range_text.c_str(), x - rangeWidth / 2, y, fontSize, MAROON);
        DrawText(sum_text.c_str(), x - sumWidth / 2, y + 20, fontSize, DARKGREEN);
        DrawText(min_text.c_str(), x - minWidth / 2, y + 40, fontSize, DARKBLUE);

        int next_y = y + 3 * radius; // Espaciado vertical según radio

        // Ajusta espaciado horizontal dinámicamente según profundidad
        float levelRatio = (float)(tree_depth - current_level + 1);
        int child_spacing = screenWidth / std::pow(2, current_level + 2);

        // Dibuja líneas y nodos hijos
        if (node->left) {
            DrawLine(x, y + radius + 30, x - child_spacing, next_y, GRAY);
            _drawNode(node->left, x - child_spacing, next_y, h_spacing / 2, current_level + 1, max_draw_level, tree_depth, screenWidth, nodes);
        }
        if (node->right) {
            DrawLine(x, y + radius + 30, x + child_spacing, next_y, GRAY);
            _drawNode(node->right, x + child_spacing, next_y, h_spacing / 2, current_level + 1, max_draw_level, tree_depth, screenWidth, nodes);
        }
    }

    

public:
    SegmentTree() : root(nullptr), size(0) {};

    ~SegmentTree() {
        deleteTree(root);
    }

    explicit SegmentTree(TNode &n, int sz) : root(&n), size(sz) {}; 
                                                                  
    explicit SegmentTree(const std::vector<T>& arr) : size(arr.size()) {
        if (arr.empty()) {
            root = nullptr;
        } else {
            root = build(arr, 0, size - 1);
        }
    }

    /**
     * @param arr 
     * @param left The left boundary of the current segment
     * @param right The right boundary of the current segment
     * @return A pointer to the root of the built segment tree for this segment
     */
    static TNode* build(const std::vector<T>& arr, int left, int right) {
        if (left == right) { // caso base: nodo hoja
            return new TNode(arr[left], left, right);
        }
        int mid = left + (right - left) / 2; 
        TNode* left_child = build(arr, left, mid);
        TNode* right_child = build(arr, mid + 1, right);
        return new TNode(left_child, right_child); // nodo interno
    }


    void rebuild(const std::vector<T>& arr) {
        deleteTree(root); 
        root = nullptr;   

        size = arr.size();
        if (!arr.empty()) {
            root = build(arr, 0, size - 1);
        }
    }

    [[nodiscard]] bool is_empty() const {
        return root == nullptr || size == 0;
    }

    SegmentTree(const SegmentTree& other) : size(other.size) {
        if (other.root) {
            root = new Node<T>(*other.root); 
        } else {
            root = nullptr;
        }
    }

    SegmentTree& operator=(const SegmentTree& other) {
        if (this != &other) {
            deleteTree(root); 
            size = other.size;
            if (other.root) {
                root = new Node<T>(*other.root); 
            } else {
                root = nullptr;
            }
        }
        return *this;
    }

    // suma [left, right]
    T query_sum(int left, int right, std::unordered_set<TNode*> &visited) {
        if (left < 0 || right >= size || left > right) {
            throw std::invalid_argument("Invalid query range");
        }
        if (is_empty()) { 
            return 0; 
        }
        return _query_sum(root, left, right, 0, size - 1, visited);
    }

    // suma [left, right]
    T query_sum(int left, int right) {
        std::unordered_set<TNode*> v;
        return query_sum(left, right, v);
    }

    TNode* update(int index, const T &value) {
        std::unordered_set<TNode*> visited;
        return update(index, value, visited);
    }

    TNode* update(int index, const T &value, std::unordered_set<TNode*> &visited) {
        if (index < 0 || index >= size) {
            throw std::invalid_argument("Invalid update index, current size is " + std::to_string(size));
        }
        if (is_empty()) {
            throw std::runtime_error("Cannot update an empty segment tree.");
        }
        return _update(root, index, value, visited);
    }

    // max en un rango
    std::optional<T> query_max(int left, int right, std::unordered_set<TNode*> &visited) {
        if (left < 0 || right >= size || left > right) {
            throw std::invalid_argument("Invalid query range");
        }
        if (is_empty()) {
            return std::nullopt; // nullpot-> empty optional
        }
        return _query_max(root, left, right, 0, size - 1, visited);
    }

    std::optional<T> query_max(int left, int right) {
        std::unordered_set<TNode*> v;
        return query_max(left, right, v);
    }

    // min en un rang
    std::optional<T> query_min(int left, int right, std::unordered_set<TNode*> &visited) {
        if (left < 0 || right >= size || left > right) {
            throw std::invalid_argument("Invalid query range");
        }
        if (is_empty()) {
            return std::nullopt; // nullpot-> empty optional
        }
        return _query_min(root, left, right, 0, size - 1, visited);
    }

    std::optional<T> query_min(int left, int right) {
        std::unordered_set<TNode*> v;
        return query_min(left, right, v);
    }

    // depth para el visu
    int getDepth() {
        return _getDepth(root);
    }
    
    void draw(int max_draw_level, std::unordered_set<TNode*> &nodes) {
        if (root) {
            // Dentro de tu método draw(...)
            int screenWidth = GetScreenWidth();
            int tree_depth = getDepth();  // ya lo tienes en Visualizer

            int initial_x = screenWidth / 2;
            int initial_y = 100;
            int initial_spacing = screenWidth / 4;

            _drawNode(root, initial_x, initial_y, initial_spacing, 0, max_draw_level, tree_depth, screenWidth, nodes);
        }
    }
    
};

#endif // SEGMENT_TREE_H