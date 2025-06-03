#include <iostream>

#include "src/segment_tree.h"

void run_test() {
    // Test data
    int data[] = {7, 2, 5, 1, 9, 3};
    SegmentTree<int> st(data);
    std::cout << "Segment tree built successfully.\n";

    // Test valid sum queries
    std::cout << "\n--- Sum queries ---\n";
    std::cout << "Sum [0:5] = " << st.query(0, 5) << " (Expected: 27)\n";
    std::cout << "Sum [1:3] = " << st.query(1, 3) << " (Expected: 8)\n";
    std::cout << "Sum [4:4] = " << st.query(4, 4) << " (Expected: 9)\n";

    // Test invalid queries with proper separation
    std::cout << "\n--- Invalid query tests ---\n";
    try {
        std::cout << "Testing invalid query [5:2]... ";
        st.query(5, 2);  // Invalid range (start > end)
        std::cout << "ERROR: Exception not thrown!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    try {
        std::cout << "Testing out-of-bounds query [-1:3]... ";
        st.query(-1, 3);  // Out of bounds
        std::cout << "ERROR: Exception not thrown!\n";
    } catch (const std::invalid_argument& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    // Test updates
    std::cout << "\n--- Update tests ---\n";
    std::cout << "Updating index 3 to 10...\n";
    st.update(3, 10);
    std::cout << "New sum [1:3] = " << st.query(1, 3) << " (Expected: 17)\n";
}

int main() {
    try {
        run_test();
        std::cout << "\nAll tests completed successfully!\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}