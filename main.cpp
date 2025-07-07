#include "raylib.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "src/segment_tree.h"


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

// Función auxiliar para convertir el string de entrada en un vector de números
std::vector<int> parseInput(const std::string& text) {
    std::vector<int> numbers;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, ' ')) { // Separa por espacios
        if (!item.empty()) {
            try {
                numbers.push_back(std::stoi(item));
            } catch (const std::exception& e) {
                std::cerr << "Entrada inválida: " << item << std::endl;
            }
        }
    }
    return numbers;
}

int main() {
    run_test();
    std::cout << "\nAll tests completed successfully!\n";

    //configuración de la ventana
    const int screenWidth = 1600;
    const int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Visualizador de Segment Tree");
    SetTargetFPS(60);

    //variables para la caja de texto
    std::string inputText = "3 1 4 2 8 5 7 6";
    Rectangle textBox = { screenWidth / 2.0f - 200, 20, 400, 40 };
    bool textBoxEditMode = false;

    //variables para la animacion del dibujo
    float animationTimer = 0.0f;
    const float ANIMATION_SPEED = 0.5f;
    int maxDrawLevel = -1;
    int treeDepth = 0;

    //construir el Segment Tree
    std::vector<int> data = parseInput(inputText);
    SegmentTree<int> tree(data);
    treeDepth = tree.getDepth();

    //bucle principal de la aplicación
    while (!WindowShouldClose()) {
        // Lógica de la caja de texto
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (CheckCollisionPointRec(GetMousePosition(), textBox)) {
                textBoxEditMode = true;
            } else {
                textBoxEditMode = false;
            }
        }

        if (textBoxEditMode) {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 125) && (inputText.length() < 99)) {
                    inputText += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && animationTimer >= 0.1f)) {
                if (!inputText.empty()) {
                    inputText.pop_back();
                    animationTimer = 0.0f;
                }
            }

            if (IsKeyPressed(KEY_ENTER)) {
                std::vector<int> newData = parseInput(inputText);
                if (!newData.empty()) {
                    tree = SegmentTree<int>(newData);
                    treeDepth = tree.getDepth();
                    maxDrawLevel = 0;
                    animationTimer = 0.0f;
                    textBoxEditMode = false;
                }
            }
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (maxDrawLevel < treeDepth) {
            animationTimer += GetFrameTime();
            if (animationTimer >= ANIMATION_SPEED) {
                animationTimer = 0.0f;
                maxDrawLevel++;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibuja el árbol
        tree.draw(maxDrawLevel);

        // Dibuja la caja de texto
        DrawRectangleRec(textBox, LIGHTGRAY);
        if (textBoxEditMode) {
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
        } else {
            DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
        }
        DrawText(inputText.c_str(), (int)textBox.x + 10, (int)textBox.y + 10, 20, MAROON);

        // Dibuja el cursor parpadeante
        if (textBoxEditMode) {
            if (((int)(GetTime() * 2)) % 2 == 0) {
                int textWidth = MeasureText(inputText.c_str(), 20);
                DrawText("_", (int)textBox.x + 10 + textWidth, (int)textBox.y + 12, 20, MAROON);
            }
        }

        // Muestra información adicional
        DrawText("Segment Tree Visualizer", 10, 10, 20, DARKGRAY);
        DrawText("Haz clic en la caja, edita los números y presiona ENTER", screenWidth / 2 - 250, 70, 10, GRAY);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}