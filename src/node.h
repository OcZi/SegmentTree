#ifndef NODE_H
#define NODE_H

#include <algorithm> // std::min and std::max
#include <stdexcept> // std::runtime_error 

template<typename T>
struct Node {
    T sum;
    Node* left;
    Node* right;
    T min, max;
    int startRange;
    int endRange;

    explicit Node(T value, int l, int r) 
        : sum(value), left(nullptr), right(nullptr), 
          min(value), max(value),
          startRange(l), endRange(r) {}

    explicit Node(Node* l, Node* r) 
        : left(l), right(r), sum(0), min(0), max(0), 
          startRange(0), endRange(0) {
        
        if (!l || !r) {
            throw std::runtime_error("Internal segment tree node must have two valid children.");
        }
        
        startRange = l->startRange;
        endRange = r->endRange;
        update(); 
    }

    // copy
    Node(const Node& other) 
        : sum(other.sum), min(other.min), max(other.max),
          startRange(other.startRange), endRange(other.endRange),
          left(nullptr), right(nullptr) { // init pointers -> nullptr
        if (other.left) {
            left = new Node(*other.left); 
        }
        if (other.right) {
            right = new Node(*other.right);
        }
    }

    // copy asign
    Node& operator=(const Node& other) {
        if (this != &other) { // Handle self-assignment
            // 1. borrar curr chil
            delete left;
            delete right;
            
            // 2. copiar
            sum = other.sum;
            min = other.min;
            max = other.max;
            startRange = other.startRange;
            endRange = other.endRange;
            
            // 3. deep copy
            left = nullptr; 
            right = nullptr; 
            if (other.left) {
                left = new Node(*other.left);
            }
            if (other.right) {
                right = new Node(*other.right);
            }
        }
        return *this;
    }

    // move
    Node(Node&& other) noexcept 
        : sum(other.sum), min(other.min), max(other.max),
          startRange(other.startRange), endRange(other.endRange),
          left(other.left), right(other.right) { // Transfer pointers
        other.left = nullptr;  
        other.right = nullptr; 
    }

    // move assign
    Node& operator=(Node&& other) noexcept {
        if (this != &other) { // Handle self-assignment
            delete left;
            delete right;
            
            sum = other.sum;
            min = other.min;
            max = other.max;
            startRange = other.startRange;
            endRange = other.endRange;
            left = other.left;
            right = other.right;
            
            // Nullify source pointers
            other.left = nullptr;
            other.right = nullptr;
        }
        return *this;
    }

    // DESTRUCTOR CORREGIDO -> NO DEBE DESTRUIRSE LEFT AND RIGHT PORQUE EL SEGMENT TREE SE ENCARGA DE ESO!!!!
    ~Node() {}

    void update() {
        if (!left && !right) return; // Hoja: ya tiene sum, min, max asignados

        sum = 0;
        min = std::numeric_limits<T>::max();
        max = std::numeric_limits<T>::min();

        if (left) {
            sum += left->sum;
            min = std::min(min, left->min);
            max = std::max(max, left->max);
        }

        if (right) {
            sum += right->sum;
            min = std::min(min, right->min);
            max = std::max(max, right->max);
        }
    }

    T operator+(const Node* n) const {
        if (!n) return sum; 
        return sum + n->sum;
    }

    T operator+(const Node& n) const {
        return sum + n.sum;
    }

    T operator-(const Node* n) const {
        if (!n) return sum; 
        return sum - n->sum;
    }

    T operator-(const Node& n) const {
        return sum - n.sum;
    }
};

#endif // NODE_H