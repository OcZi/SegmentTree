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

    SegmentTree<int> tree; // no hacer el objeto innecesariamente un puntero a un tree, doble manejo innc 
    int treeDepth;

    std::string inputText;
    Rectangle textBox;
    bool textBoxEditMode;
    float animationTimer;
    int maxDrawLevel;

    void Update();
    void Draw();
    void rebuildTree(const std::vector<int>& newData); // nuevo método: construir el árbol con nuevos datos sin tener el cuenta el default
};

#endif //VISUALIZER_H
