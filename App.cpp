#include "Config.h"
#include "SnakeGame.h"

#include <string>
#include <format>
#include <algorithm>

static void DrawGridOnBackground(const Field2& grid)
{
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
    ImVec2 screen_size = ImGui::GetIO().DisplaySize;

    // Адаптивный размер клеток (занимаем 80% экрана)
    float max_grid_width = screen_size.x * 0.8f;
    float max_grid_height = screen_size.y * 0.8f;
    float cellSize = max_grid_height / grid.GetHeight();
    if (cellSize > max_grid_width / grid.GetWidth())
        cellSize = max_grid_width / grid.GetWidth();

    float grid_width = grid.GetWidth() * cellSize;
    float grid_height = grid.GetHeight() * cellSize;

    ImVec2 grid_pos = ImVec2(
        (screen_size.x - grid_width) * 0.5f,
        (screen_size.y - grid_height) * 0.5f
    );

    // Градиентный фон
    for (float y = 0; y < screen_size.y; y += 2.0f)
    {
        float alpha = 30 + (y / screen_size.y) * 20; // Градиент сверху вниз
        draw_list->AddLine(
            ImVec2(0, y),
            ImVec2(screen_size.x, y),
            IM_COL32(25, 25, 35, (int)alpha)
        );
    }

    // Остальной код отрисовки клеток...
    for (int y = 0; y < grid.GetHeight(); ++y)
    {
        for (int x = 0; x < grid.GetWidth(); ++x)
        {
            ImVec2 rect_min = ImVec2(
                grid_pos.x + x * cellSize,
                grid_pos.y + y * cellSize
            );
            ImVec2 rect_max = ImVec2(
                rect_min.x + cellSize,
                rect_min.y + cellSize
            );

            char cell = grid.GetCell(UVec2{
                static_cast<unsigned char>(y),
                static_cast<unsigned char>(x)
                });

            ImColor color;
            switch (cell)
            {
            case Cell::EMPTY: color = Color::EMPTY; break;
            case Cell::SNAKE_HEAD: color = Color::SNAKE_HEAD; break;
            case Cell::SNAKE_BODY: color = Color::SNAKE_BODY; break;
            case Cell::APPLE: color = Color::APPLE; break;
            default: color = Color::DEFAULT; break;
            }

            draw_list->AddRectFilled(rect_min, rect_max, color);
            draw_list->AddRect(rect_min, rect_max, Color::WHITE);
        }
    }
}

static void DrawAndHandleOverlay(const SnakeGame& snake)
{
    if (snake.GetGameState() == SnakeGame::Running)
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);

    ImVec2 screen_size = ImGui::GetIO().DisplaySize;
    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    draw_list->AddRectFilled(ImVec2(0, 0), screen_size, IM_COL32(0, 0, 0, 180));

    // Центральное окно с сообщением
    ImGui::SetNextWindowPos(ImVec2(screen_size.x * 0.5f, screen_size.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(400, 200));

    if (ImGui::Begin("Game Overlay", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {
        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        switch (snake.GetGameState())
        {
        case SnakeGame::GameWin:
            textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImGui::TextColored(textColor, "CONGRATULATIONS!");
            ImGui::Text("You won the game!");
            break;
        case SnakeGame::GameOver:
            textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImGui::TextColored(textColor, "GAME OVER");
            break;
        case SnakeGame::Paused:
            textColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImGui::TextColored(textColor, "GAME PAUSED");
            break;
        }

        ImGui::Separator();
        ImGui::Text("Score: %zu / %zu", snake.GetCurrentScore(), snake.GetTargetScore());
        ImGui::Spacing();

        // Кнопка рестарта
        if (ImGui::Button("Restart Game", ImVec2(120, 40)))
        {
            ImGui::GetIO().AddKeyEvent(ImGuiKey_LeftCtrl, true);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_R, true);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_R, false);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_LeftCtrl, false);
        }

        ImGui::SameLine();

        // Кнопка продолжения (только для паузы)
        if (
            (snake.GetGameState() == SnakeGame::Paused)
            && ImGui::Button("Continue", ImVec2(120, 40))
            )
        {
            ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, true);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, false);
        }

        ImGui::Spacing();
        ImGui::Text("Press ESC to toggle pause");
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

namespace App
{
    static SnakeGame snakeGame(12, 16);

	static void Init()
	{
	}

	static void Update()
	{
        snakeGame.ApplyFrameDelay();
        ImGui::SetNextFrameWantCaptureKeyboard(true);
        snakeGame.ProcessUserInput();
        snakeGame.Update();
        DrawGridOnBackground(snakeGame.GetFrame());
        DrawAndHandleOverlay(snakeGame);
	}

	static void Shutdown()
	{
	}
}