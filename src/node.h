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

    //estos atributos sirven para visualizar el rango de cada nodo
    int startRange;
    int endRange;


    explicit Node(T value, int l, int r)
        : sum(value), min_val(value), max_val(value),
          left(nullptr), right(nullptr), startRange(l), endRange(r) {};

    explicit Node(Node* l, Node* r)
        : left(l), right(r) {
        update();
    };

    void update() {
        startRange = left->startRange;
        endRange = right->endRange;

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
