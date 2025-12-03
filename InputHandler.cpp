#include "InputHandler.h"
#include "Direction.h"
#include "imgui.h"

void InputHandler::processGameInput(SnakeGame& game)
{
    handleControlInput(game);
    handleDirectionInput(game);
}

void InputHandler::handleControlInput(SnakeGame& game)
{
    // Рестарт по Ctrl+R
    if (ImGui::IsKeyDown(ImGuiKey_R) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        game.restart();

    // Пауза по ESC
    if (ImGui::IsKeyPressed(ImGuiKey_Escape, false))
        game.togglePause();
}

void InputHandler::handleDirectionInput(SnakeGame& game)
{
    if (ImGui::IsKeyPressed(ImGuiKey_W) || ImGui::IsKeyPressed(ImGuiKey_UpArrow))
        game.changeDirection(Direction::UP);
    else if (ImGui::IsKeyPressed(ImGuiKey_S) || ImGui::IsKeyPressed(ImGuiKey_DownArrow))
        game.changeDirection(Direction::DOWN);
    else if (ImGui::IsKeyPressed(ImGuiKey_A) || ImGui::IsKeyPressed(ImGuiKey_LeftArrow))
        game.changeDirection(Direction::LEFT);
    else if (ImGui::IsKeyPressed(ImGuiKey_D) || ImGui::IsKeyPressed(ImGuiKey_RightArrow))
        game.changeDirection(Direction::RIGHT);
}