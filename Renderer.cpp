#include "Renderer.h"
#include <format>

void Renderer::drawBackground()
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    // Градиентный фон
    for (float y = 0; y < screenSize.y; y += 2.0f)
    {
        float alpha = 30 + (y / screenSize.y) * 20;
        drawList->AddLine(
            ImVec2(0, y),
            ImVec2(screenSize.x, y),
            IM_COL32(25, 25, 35, static_cast<int>(alpha))
        );
    }
}

ImVec2 Renderer::calculateGridPosition(const Field2& grid, float& cellSize)
{
    ImVec2 screenSize = ImGui::GetIO().DisplaySize;

    // Адаптивный размер клеток (80% экрана)
    float maxGridWidth = screenSize.x * 0.8f;
    float maxGridHeight = screenSize.y * 0.8f;

    cellSize = maxGridHeight / grid.getHeight();
    if (cellSize > maxGridWidth / grid.getWidth())
        cellSize = maxGridWidth / grid.getWidth();

    float gridWidth = grid.getWidth() * cellSize;
    float gridHeight = grid.getHeight() * cellSize;

    return ImVec2
    (
        (screenSize.x - gridWidth) * 0.5f,
        (screenSize.y - gridHeight) * 0.5f
    );
}

void Renderer::drawGridCells(const Field2& grid, const ImVec2& gridPos, float cellSize)
{
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    for (unsigned char row = 0; row < grid.getHeight(); row++)
        for (unsigned char col = 0; col < grid.getWidth(); col++)
        {
            ImVec2 rectMin = ImVec2
            (
                gridPos.x + col * cellSize,
                gridPos.y + row * cellSize
            );
            ImVec2 rectMax = ImVec2
            (
                rectMin.x + cellSize,
                rectMin.y + cellSize
            );

            char cell = grid.getCell({ row, col });
            ImColor color;

            switch (cell)
            {
            case CellSymbol::EMPTY: color = Color::EMPTY; break;
            case CellSymbol::SNAKE_HEAD: color = Color::SNAKE_HEAD; break;
            case CellSymbol::SNAKE_BODY: color = Color::SNAKE_BODY; break;
            case CellSymbol::APPLE: color = Color::APPLE; break;
            default: color = Color::DEFAULT; break;
            }

            drawList->AddRectFilled(rectMin, rectMax, color);
            drawList->AddRect(rectMin, rectMax, Color::WHITE);
        }
}

void Renderer::drawGrid(const Field2& grid)
{
    drawBackground();

    float cellSize;
    ImVec2 gridPos = calculateGridPosition(grid, cellSize);
    drawGridCells(grid, gridPos, cellSize);
}

void Renderer::drawGameOverlay(const SnakeGame& game)
{
    if (game.getGameState() == GameState::Running)
        return;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.8f);

    ImVec2 screenSize = ImGui::GetIO().DisplaySize;
    ImDrawList* drawList = ImGui::GetBackgroundDrawList();

    drawList->AddRectFilled(ImVec2(0, 0), screenSize, Color::OVERLAY_BG);

    ImGui::SetNextWindowPos
    (
        ImVec2(screenSize.x * 0.5f, screenSize.y * 0.5f),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.5f)
    );
    ImGui::SetNextWindowSize(ImVec2(400, 200));

    if (ImGui::Begin("Game Overlay", nullptr,
        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {

        ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        const char* title = "";
        const char* message = "";

        switch (game.getGameState())
        {
        case GameState::GameWin:
            textColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            title = "GAME WIN";
            message = "You won the game!";
            break;
        case GameState::GameOver:
            textColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            title = "GAME OVER";
            message = "";
            break;
        case GameState::Paused:
            textColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            title = "GAME PAUSED";
            message = "";
            break;
        default:
            break;
        }

        // Центрирование текста
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(title).x) * 0.5f);
        ImGui::TextColored(textColor, "%s", title);

        if (message[0] != '\0')
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(message).x) * 0.5f);
            ImGui::Text("%s", message);
        }

        ImGui::Separator();

        // Счет
        ImGui::Text("Score: %zu / %zu", game.getCurrentScore(), game.getTargetScore());
        ImGui::Spacing();

        // Кнопки (их обработка будет через стандартный ввод)
        float buttonWidth = 120.0f;
        float totalWidth = buttonWidth * 2 + ImGui::GetStyle().ItemSpacing.x;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);

        // Кнопка рестарта
        if (ImGui::Button("Restart Game", ImVec2(buttonWidth, 40)))
        {
            // Создаем события клавиш для рестарта
            ImGui::GetIO().AddKeyEvent(ImGuiKey_LeftCtrl, true);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_R, true);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_R, false);
            ImGui::GetIO().AddKeyEvent(ImGuiKey_LeftCtrl, false);
        }

        ImGui::SameLine();

        // Кнопка продолжения (только для паузы)
        if (game.getGameState() == GameState::Paused)
            if (ImGui::Button("Continue", ImVec2(buttonWidth, 40)))
            {
                // Создаем событие клавиши ESC
                ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, true);
                ImGui::GetIO().AddKeyEvent(ImGuiKey_Escape, false);
            }

        ImGui::Spacing();
        ImGui::Text("Press ESC to toggle pause");
        ImGui::Text("Press Ctrl+R to restart");
    }
    ImGui::End();
    ImGui::PopStyleVar();
}