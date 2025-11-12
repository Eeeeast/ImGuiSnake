#include <iostream>
#include <thread>
#include "SnakeGame.h"
#include <cassert>
#include "Config.h"

SnakeGame::SnakeGame(unsigned char boardWidth, unsigned char boardHeight)
    : frame{ Field2(boardWidth, boardHeight, Cell::EMPTY) },
    currentScore{ 1 },
    targetScore{ (size_t)boardWidth * boardHeight },
    gameState{ GameState::Paused },
    snakeDir{ Field2::DIR_DOWN },
    nextSnakeDir{ Field2::DIR_DOWN }
{
    assert(boardWidth > 1 && "The field is too small");
    assert(boardHeight > 1 && "The field is too small");

    frame.Clear();
    snakeBody.push_back(*frame.TryGetCenterPos());
    frame.SetCell(snakeBody.front(), Cell::SNAKE_HEAD);
    apple = frame.TryFindFreeRandomPos();
    frame.SetCell(*apple, Cell::APPLE);
}

void SnakeGame::ApplyFrameDelay() const
{
    constexpr int BASE_DELAY_MS = 250;
    constexpr int MIN_DELAY_MS = 100;
    constexpr int REDUCTION_STEP = 10;
    std::chrono::milliseconds delay = std::chrono::milliseconds(std::max(
        MIN_DELAY_MS,
        BASE_DELAY_MS - REDUCTION_STEP * (int)currentScore
    ));
    std::this_thread::sleep_for(delay);
}

void SnakeGame::ChangeSnakeDir(Vec2 desiredDir)
{
    if (gameState == GameState::Paused)
        gameState = GameState::Running;

    bool isOpposite =
        snakeDir.first == -desiredDir.first
        && snakeDir.second == -desiredDir.second;

    if (snakeBody.size() > 1 && isOpposite)
        return;
    nextSnakeDir = desiredDir;
}

void SnakeGame::ProcessUserInput()
{
    ImGuiIO& io = ImGui::GetIO();

    if (ImGui::IsKeyDown(ImGuiKey_R) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
        RestartGame();
    if (ImGui::IsKeyDown(ImGuiKey_Escape)) HandlePause();
    if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow))
        ChangeSnakeDir(Field2::DIR_UP);
    if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow))
        ChangeSnakeDir(Field2::DIR_DOWN);
    if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow))
        ChangeSnakeDir(Field2::DIR_LEFT);
    if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow))
        ChangeSnakeDir(Field2::DIR_RIGHT);
}

void SnakeGame::Update()
{
    if (gameState != GameState::Running)
        return;

    snakeDir = nextSnakeDir;
    UVec2 newHead = frame.GetShiftedPosition(snakeBody.front(), nextSnakeDir);

    switch (frame.GetCell(newHead))
    {
    case Cell::APPLE:
        currentScore++;
        apple = frame.TryFindFreeRandomPos();
        if (!apple) // ≈сли не осталось свободных мест - победа
            gameState = GameState::GameWin;
        break;
    case Cell::EMPTY:
        snakeBody.pop_back();
        break;
    default:
        gameState = currentScore == targetScore ? GameState::GameWin : GameState::GameOver;
        return;
    }

    snakeBody.push_front(newHead);

    frame.Clear();
    for (UVec2 t : snakeBody)
        frame.SetCell(t, Cell::SNAKE_BODY);
    frame.SetCell(snakeBody.front(), Cell::SNAKE_HEAD);
    if (apple)
        frame.SetCell(*apple, Cell::APPLE);
}

void SnakeGame::HandlePause()
{
    if (gameState == GameState::Running)
        gameState = GameState::Paused;
    else if (gameState == GameState::Paused)
        gameState = GameState::Running;
}

void SnakeGame::RestartGame()
{
    frame.Clear();
    snakeBody.clear();
    currentScore = 1;
    gameState = GameState::Running;
    snakeDir = Field2::DIR_DOWN;
    nextSnakeDir = snakeDir;

    snakeBody.push_back(*frame.TryGetCenterPos());
    frame.SetCell(snakeBody.front(), Cell::SNAKE_HEAD);
    apple = frame.TryFindFreeRandomPos();
    frame.SetCell(*apple, Cell::APPLE);
}