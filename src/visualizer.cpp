//
// Created by dalev on 7/07/2025.
//

#include <iostream>

#include "visualizer.h"
#include <sstream>
#include "screens.h"

GameScreen currentScreen = LOGO;

static std::string DEFAULT_TEXT_BOX = "3 1 4 2 8 5 7 6";
static std::pair DEFAULT_INFO_TEXT = {"Haz clic en la caja, edita los números y presiona ENTER", GRAY};

// Función auxiliar para convertir el string de entrada en un vector de números
std::vector<int> parseInput(const std::string& text)
{
    std::vector<int> numbers;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, ' '))
    {
        // Separa por espacios
        if (!item.empty())
        {
            try
            {
                numbers.push_back(std::stoi(item));
            }
            catch (const std::exception& e)
            {
                std::cerr << "Entrada inválida: " << item << std::endl;
            }
        }
    }
    return numbers;
}

void Visualizer::rebuildTree(const std::vector<int>& newData)
{
    tree.rebuild(newData); // usamos el método rebuild del SegmentTree -> construir a partir de un vector!
    treeDepth = tree.getDepth();
    maxDrawLevel = 0;
    animationTimer = 0.0f;
}


Visualizer::Visualizer(int width, int height, const char* title)
    : screenWidth(width), screenHeight(height), appTitle(title)
{
    // Inicializamos el estado
    inputText = DEFAULT_TEXT_BOX;
    infoText = DEFAULT_INFO_TEXT;
    textBox = {width / 2.0f - 200, 20, 400, 40};
    buildButton = {screenWidth / 2.0f - 200, 100, 100, 30};
    updateButton = {screenWidth / 2.0f + 100, 100, 100, 30};
    sumQueryButton   = {screenWidth / 2.0f - 200, 150, 100, 30};
    maxQueryButton   = {screenWidth / 2.0f + 210, 150, 100, 30};
    minQueryButton   = {screenWidth / 2.0f + 100, 150, 100, 30};

    textBoxEditMode = false;
    animationTimer = 0.0f;
    maxDrawLevel = 0;

    treeDepth = tree.getDepth();
}

Visualizer::~Visualizer()
{
    CloseWindow();
}

void Visualizer::Run()
{
    currentScreen = LOGO;
    InitWindow(screenWidth, screenHeight, appTitle);

    InitLogoScreen();
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }
}

void Visualizer::Update()
{
    if (currentScreen == LOGO)
    {
        UpdateLogoScreen();
        if (FinishLogoScreen()) currentScreen = VISUALIZER;
        return;
    }

    constexpr float ANIMATION_SPEED = 0.5f;

    Vector2 mouse = GetMousePosition();
    try
    {
        // Modo edición del textbox
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            textBoxEditMode = CheckCollisionPointRec(mouse, textBox);

            // Botón Construir
            if (CheckCollisionPointRec(mouse, buildButton))
            {
                std::vector<int> newData = parseInput(inputText);
                if (!newData.empty()) rebuildTree(newData);
                infoText = {"Nodos totales: " + std::to_string(newData.size()), GRAY};
            }

            // Botón Update
            if (CheckCollisionPointRec(mouse, updateButton))
            {
                int idx, val;
                if (sscanf(inputText.c_str(), "%d %d", &idx, &val) == 2)
                {
                    std::unordered_set<Node<int>*> n;
                    auto r = tree.update(idx, val, n);
                    visitedNodes = n;

                    if (r)
                    {
                        infoText = {"Nodo actualizado: " + std::to_string(r->sum), BLUE};
                    }

                    animationTimer = 0.0f;
                    maxDrawLevel = 0;
                }
            }

            if (CheckCollisionPointRec(mouse, sumQueryButton))
            {
                int l, r;
                if (sscanf(inputText.c_str(), "%d %d", &l, &r) == 2)
                {
                    std::unordered_set<Node<int>*> visited;
                    int res = tree.query_sum(l, r, visited);
                    visitedNodes = visited;

                    infoText = {"Query sum: " + std::to_string(res), GOLD};
                    animationTimer = 0.0f;
                    maxDrawLevel = 0;
                }
            }

            if (CheckCollisionPointRec(mouse, maxQueryButton))
            {
                int l, r;
                if (sscanf(inputText.c_str(), "%d %d", &l, &r) == 2)
                {
                    std::unordered_set<Node<int>*> visited;
                    auto result = tree.query_max(l, r, visited);
                    visitedNodes = visited;

                    if (result.has_value())
                    {
                        std::cout << "max: " << result.value() << std::endl;
                        infoText = {"Query max: " + std::to_string(result.value()), GOLD};
                    }

                    animationTimer = 0.0f;
                    maxDrawLevel = 0;
                }
            }

            if (CheckCollisionPointRec(mouse, minQueryButton))
            {
                int l, r;
                if (sscanf(inputText.c_str(), "%d %d", &l, &r) == 2)
                {
                    std::unordered_set<Node<int>*> visited;
                    auto result = tree.query_min(l, r, visited);
                    visitedNodes = visited;

                    if (result.has_value())
                    {
                        std::cout << "min: " << result.value() << std::endl;
                        infoText = {"Query min: " + std::to_string(result.value()), GOLD};
                    }

                    animationTimer = 0.0f;
                    maxDrawLevel = 0;
                }
            }


            /*
             *
    if (CheckCollisionPointRec(GetMousePosition(), updateButton) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        // por ejemplo cambiar valor en la posición 3 a 99
        tree.update(3, 99);

        treeDepth = tree.getDepth();        // por si cambió el árbol
        maxDrawLevel = 0;                   // reinicia la animación
        animationTimer = 0.0f;
    }
            */
        }

        if (textBoxEditMode)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (inputText.length() < 99))
                {
                    inputText += (char)key;
                }
                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE) || (IsKeyDown(KEY_BACKSPACE) && animationTimer >= 0.1f))
            {
                if (!inputText.empty())
                {
                    inputText.pop_back();
                    animationTimer = 0.0f;
                }
            }
        }
        else
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }
    } catch (...)
    {
        auto p = std::current_exception();
        auto s = (p ? p.__cxa_exception_type()->name() : "null");
        infoText = {s, RED};
    }

    if (maxDrawLevel < treeDepth)
    {
        animationTimer += GetFrameTime();
        if (animationTimer >= ANIMATION_SPEED)
        {
            animationTimer = 0.0f;
            maxDrawLevel++;
        }
    }
}


void Visualizer::Draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);
    if (currentScreen == LOGO)
    {
        DrawLogoScreen();
    }
    if (currentScreen == VISUALIZER)
    {
        // Dibuja el árbol
        tree.draw(maxDrawLevel, visitedNodes);

        // Dibuja la caja de texto
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawRectangleLinesEx(textBox, 2, textBoxEditMode ? RED : DARKGRAY);
        DrawText(inputText.c_str(), (int)textBox.x + 10, (int)textBox.y + 10, 20, MAROON);

        if (textBoxEditMode && (((int)(GetTime() * 2)) % 2 == 0))
        {
            int textWidth = MeasureText(inputText.c_str(), 20);
            DrawText("_", (int)textBox.x + 10 + textWidth, (int)textBox.y + 12, 20, MAROON);
        }

        // Botones
        DrawRectangleRec(buildButton, DARKGRAY);
        DrawText("Construir", (int)buildButton.x + 10, (int)buildButton.y + 7, 18, WHITE);

        DrawRectangleRec(updateButton, DARKGRAY);
        DrawText("Actualizar", (int)updateButton.x + 10, (int)updateButton.y + 7, 18, WHITE);

        DrawRectangleRec(sumQueryButton, DARKGRAY);
        DrawText("Query Sum", (int)sumQueryButton.x + 10, (int)sumQueryButton.y + 7, 18, WHITE);

        DrawRectangleRec(maxQueryButton, DARKGRAY);
        DrawText("Query Max", (int)maxQueryButton.x + 10, (int)maxQueryButton.y + 7, 18, WHITE);

        DrawRectangleRec(minQueryButton, DARKGRAY);
        DrawText("Query Min", (int)minQueryButton.x + 10, (int)minQueryButton.y + 7, 18, WHITE);

        // Info
        DrawText(appTitle, 10, 10, 20, DARKGRAY);
        DrawText(infoText.first.c_str(), (int)textBox.x, (int)textBox.y + 50, 16,
                 infoText.second);
    }

    EndDrawing();
}
