#pragma once

#include "SnakeGame.h"

class InputHandler
{
public:
    static void processGameInput(SnakeGame& game);

private:
    static void handleDirectionInput(SnakeGame& game);
    static void handleControlInput(SnakeGame& game);
};