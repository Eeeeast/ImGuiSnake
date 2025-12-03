#include "App.h"
#include "SnakeGame.h"
#include "InputHandler.h"
#include "Renderer.h"
#include "imgui.h"

namespace {
    SnakeGame snakeGame(12, 16);
}

void App::Init() {
    // Инициализация при необходимости
}

void App::Update() {
    snakeGame.applyFrameDelay();

    // Обработка ввода
    InputHandler::processGameInput(snakeGame);

    // Обновление игры
    snakeGame.update();

    // Отрисовка
    Renderer::drawGrid(snakeGame.getFrame());
    Renderer::drawGameOverlay(snakeGame);
}

void App::Shutdown() {
    // Очистка ресурсов при необходимости
}