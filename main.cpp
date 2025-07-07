#include "raylib.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "src/segment_tree.h"
#include "src/visualizer.h"

void run_test() {
    // Test data
    std::vector<int> data = {7, 2, 5, 1, 9, 3};
    SegmentTree<int> st(data);
    std::cout << "Segment tree built successfully.\n";

    // Test valid sum queries
    std::cout << "\n--- Sum queries ---\n";
    std::cout << "Sum [0:5] = " << st.query_sum(0, 5) << " (Expected: 27)\n";
    std::cout << "Sum [1:3] = " << st.query_sum(1, 3) << " (Expected: 8)\n";
    std::cout << "Sum [4:4] = " << st.query_sum(4, 4) << " (Expected: 9)\n";

    // Test min/max queries
    std::cout << "\n--- Min/Max queries ---\n";
    std::cout << "Min [0:5] = " << st.query_min(0, 5).value() << " (Expected: 1)\n";
    std::cout << "Max [0:5] = " << st.query_max(0, 5).value() << " (Expected: 9)\n";
    std::cout << "Min [1:3] = " << st.query_min(1, 3).value() << " (Expected: 1)\n";
    std::cout << "Max [1:3] = " << st.query_max(1, 3).value() << " (Expected: 5)\n";
    std::cout << "Min [4:4] = " << st.query_min(4, 4).value() << " (Expected: 9)\n";
    std::cout << "Max [4:4] = " << st.query_max(4, 4).value() << " (Expected: 9)\n";


    // Test invalid queries with proper separation
    std::cout << "\n--- Invalid query tests ---\n";
    try {
        std::cout << "Testing invalid query [5:2]... ";
        st.query_sum(5, 2);  // Invalid range (start > end)
        std::cout << "ERROR: Exception not thrown!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    try {
        std::cout << "Testing out-of-bounds query [-1:3]... ";
        st.query_sum(-1, 3);  // Out of bounds
        std::cout << "ERROR: Exception not thrown!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    // Test updates
    std::cout << "\n--- Update tests ---\n";
    std::cout << "Updating index 3 to 10...\n";
    st.update(3, 10);
    std::cout << "New sum [1:3] = " << st.query_sum(1, 3) << " (Expected: 17)\n";
    std::cout << "New min [1:3] = " << st.query_min(1, 3).value() << " (Expected: 2)\n";
    std::cout << "New max [1:3] = " << st.query_max(1, 3).value() << " (Expected: 10)\n";
}

int main() {
    run_test(); // Opcional: ejecutar pruebas de consola

    Visualizer app(1600, 900, "Visualizador de Segment Tree");
    app.Run();

    return 0;
}