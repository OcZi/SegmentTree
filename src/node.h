//
// Created by Salva on 31/05/25.
//

#ifndef NODE_H
#define NODE_H



template<typename T>
struct Node {
    T sum{};
    Node* left ,right;

    explicit Node(T sum) : sum(sum), left(nullptr), right(nullptr) {};

    explicit Node(Node* l, Node* r) : left(l), right(r) { update(); };

    void update() {
        if (left) *(this) += left;
        if (right) *(this) += right;
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
