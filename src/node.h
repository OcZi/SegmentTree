//
// Created by Salva on 31/05/25.
//

#ifndef NODE_H
#define NODE_H
#include <algorithm>


template<typename T>
struct Node {
    T sum{};
    Node* left;
    Node* right;
    T min_val, max_val; // añadir valores min y max para queries;


    explicit Node(T value)
        : sum(value), min_val(value), max_val(value),
          left(nullptr), right(nullptr) {};

    explicit Node(Node* l, Node* r)
        : left(l), right(r) {
        update();
    };

    void update() {
        sum = left->sum + right->sum;
        min_val = std::min(left->min_val, right->min_val);
        max_val = std::max(left->max_val, right->max_val);
    }

    T operator+(Node* n) {
        return sum + n->sum;
    }

    T operator+(Node& n) {
        return sum + n.sum;
    }

    T operator-(Node* n) {
        return sum - n->sum;
    }

    T operator-(Node& n) {
        return sum - n.sum;
    }
};



#endif //NODE_H
