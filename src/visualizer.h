//
// Created by dalev on 7/07/2025.
//

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "raylib.h"
#include "segment_tree.h"
#include <string>

class Visualizer {
public:
    Visualizer(int width, int height, const char* title);
    ~Visualizer();
    void Run();

private:
    int screenWidth;
    int screenHeight;

    SegmentTree<int> *tree;
    int treeDepth;

    std::string inputText;
    Rectangle textBox;
    bool textBoxEditMode;
    float animationTimer;
    int maxDrawLevel;

    void Update();
    void Draw();
};

#endif //VISUALIZER_H
