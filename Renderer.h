#pragma once

#include "Field2.h"
#include "SnakeGame.h"
#include "Colors.h"

class Renderer
{
public:
    static void drawGrid(const Field2& grid);
    static void drawGameOverlay(const SnakeGame& game);
    static void drawBackground();

private:
    static void drawGridCells(const Field2& grid, const ImVec2& gridPos, float cellSize);
    static ImVec2 calculateGridPosition(const Field2& grid, float& cellSize);
};