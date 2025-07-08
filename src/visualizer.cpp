//
// Created by dalev on 7/07/2025.
//

#include <iostream>

#include "visualizer.h"
#include <sstream>
#include "screens.h"

GameScreen currentScreen = LOGO;

static std::string DEFAULT_TEXT_BOX = "3 1 4 2 8 5 7 6";

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

void Visualizer::rebuildTree(const std::vector<int>& newData) {
    tree.rebuild(newData);  // usamos el método rebuild del SegmentTree -> construir a partir de un vector!
    treeDepth = tree.getDepth();
    maxDrawLevel = 0;
    animationTimer = 0.0f;
}


Visualizer::Visualizer(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height), appTitle(title)
{
    // Inicializamos el estado
    inputText = DEFAULT_TEXT_BOX;
    textBox = { width / 2.0f - 200, 20, 400, 40 };
    textBoxEditMode = false;
    animationTimer = 0.0f;
    maxDrawLevel = 0;

    treeDepth = tree.getDepth();
}

Visualizer::~Visualizer() {
    CloseWindow();
}

void Visualizer::Run() {
    currentScreen = LOGO;
    InitWindow(screenWidth, screenHeight, appTitle);

    InitLogoScreen();
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        Update();
        Draw();
    }
}

void Visualizer::Update() {
    if (currentScreen == LOGO)
    {
        UpdateLogoScreen();
        if (FinishLogoScreen()) currentScreen = VISUALIZER;
        return;
    }

    constexpr float ANIMATION_SPEED = 0.5f;

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        textBoxEditMode = CheckCollisionPointRec(GetMousePosition(), textBox);
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
                rebuildTree(newData);  
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
}

void _Draw()
{

}

void Visualizer::Draw() {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    if (currentScreen == LOGO)
    {
        DrawLogoScreen();
    }
    if (currentScreen == VISUALIZER)
    {
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
        DrawText(appTitle, 10, 10, 20, DARKGRAY);
        DrawText("Haz clic en la caja, edita los números y presiona ENTER", screenWidth / 2 - 250, 70, 10, GRAY);
    }

    EndDrawing();
}
