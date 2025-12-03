#include "SnakeGame.h"
#include "Direction.h"
#include "Colors.h"
#include <thread>
#include <chrono>
#include <cassert>
#include <iostream>

SnakeGame::SnakeGame(unsigned char boardWidth, unsigned char boardHeight)
    : m_frame(boardWidth, boardHeight, ' '),
    m_targetScore((size_t)(boardWidth) * boardHeight)
{

    assert(boardWidth > 1 && "Board width must be greater than 1");
    assert(boardHeight > 1 && "Board height must be greater than 1");

    restart();
}

void SnakeGame::applyFrameDelay() const
{
    constexpr int BASE_DELAY_MS = 250;
    constexpr int MIN_DELAY_MS = 100;
    constexpr int REDUCTION_STEP = 10;

    int delayMs = std::max
    (
        MIN_DELAY_MS,
        BASE_DELAY_MS - REDUCTION_STEP * (int)(m_currentScore)
    );

    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
}

void SnakeGame::changeDirection(Vec2 desiredDir)
{
    if (m_gameState == GameState::Paused)
        m_gameState = GameState::Running;

    if (m_snakeBody.size() > 1 && Direction::areOpposite(m_snakeDir, desiredDir))
        return;
    m_nextSnakeDir = desiredDir;
}

void SnakeGame::processUserInput()
{
    // Ввод обрабатывается в App.cpp через вызовы public методов
}

void SnakeGame::update()
{
    if (m_gameState != GameState::Running)
        return;

    m_snakeDir = m_nextSnakeDir;
    moveSnake();
}

void SnakeGame::moveSnake()
{
    UVec2 newHead = m_frame.getShiftedPosition(m_snakeBody.front(), m_nextSnakeDir);

    checkCollision(newHead);

    if (m_gameState != GameState::Running)
        return;

    m_snakeBody.push_front(newHead);

    // Обновляем игровое поле
    m_frame.clear();
    for (const auto& segment : m_snakeBody)
        m_frame.setCell(segment, CellSymbol::SNAKE_BODY);
    m_frame.setCell(m_snakeBody.front(), CellSymbol::SNAKE_HEAD);

    if (m_apple)
        m_frame.setCell(*m_apple, CellSymbol::APPLE);
}

void SnakeGame::checkCollision(const UVec2& newHead)
{
    char cell = m_frame.getCell(newHead);

    switch (cell)
    {
    case CellSymbol::APPLE:
        m_currentScore++;
        spawnApple();
        if (!m_apple)
            m_gameState = GameState::GameWin;
        break;

    case CellSymbol::EMPTY:
        m_snakeBody.pop_back();
        break;

    default: // Столкновение с собой
        m_gameState = (m_currentScore == m_targetScore)
            ? GameState::GameWin
            : GameState::GameOver;
        break;
    }
}

void SnakeGame::spawnApple()
{
    m_apple = m_frame.tryFindFreeRandomPos();
    if (m_apple)
        m_frame.setCell(*m_apple, CellSymbol::APPLE);
}

void SnakeGame::togglePause()
{
    if (m_gameState == GameState::Running)
        m_gameState = GameState::Paused;
    else if (m_gameState == GameState::Paused)
        m_gameState = GameState::Running;
}

void SnakeGame::restart()
{
    m_frame.clear();
    m_snakeBody.clear();
    m_currentScore = 1;
    m_gameState = GameState::Running;
    m_snakeDir = Direction::DOWN;
    m_nextSnakeDir = Direction::DOWN;

    m_snakeBody.push_back(*m_frame.tryGetCenterPos());
    m_frame.setCell(m_snakeBody.front(), CellSymbol::SNAKE_HEAD);
    spawnApple();
}