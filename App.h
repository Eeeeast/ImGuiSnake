#pragma once

#include "SnakeGame.h"
#include "Renderer.h"

namespace App
{
    void Init();
    void Update();
    void Shutdown();

    // Обработчики ввода
    void HandleKeyPress(int key);
    void HandleRestart();
    void HandleTogglePause();
}