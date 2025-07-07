#include <iostream>

#include "src/segment_tree.h"
#include "raylib.h"

void run_test() {
    // Test data
    int data[] = {7, 2, 5, 1, 9, 3};
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
    try {
        run_test();
        std::cout << "\nAll tests completed successfully!\n";

        //configuración de la ventana
        const int screenWidth = 1600;
        const int screenHeight = 900;
        InitWindow(screenWidth, screenHeight, "Visualizador de Segment Tree");
        SetTargetFPS(60);

        //datos de entrada
        int data[] = {3, 1, 4, 2, 8, 5, 7, 6};

        //construir el Segment Tree
        SegmentTree<int> tree(data);

        //bucle principal de la aplicación
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(RAYWHITE);

            // Dibuja el árbol
            tree.draw();

            // Muestra información adicional
            DrawText("Segment Tree Visualizer", 10, 10, 20, DARKGRAY);

            std::string original_array = "Arreglo original: ";
            for(int val : data) original_array += std::to_string(val) + " ";
            DrawText(original_array.c_str(), 10, 40, 20, BLACK);


            EndDrawing();
        }

        CloseWindow();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << "\n";
        return 1;
    }
}