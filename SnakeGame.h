#pragma once

#include "GameState.h"
#include "Field2.h"
#include "Vec2.h"
#include <deque>
#include <optional>
#include <cstddef>

class SnakeGame
{
public:
    SnakeGame(unsigned char boardWidth, unsigned char boardHeight);

    void applyFrameDelay() const;
    void processUserInput();
    void update();

    GameState getGameState() const { return m_gameState; }
    size_t getCurrentScore() const { return m_currentScore; }
    size_t getTargetScore() const { return m_targetScore; }
    const Field2& getFrame() const { return m_frame; }

    void changeDirection(Vec2 desiredDir);
    void togglePause();
    void restart();

private:
    void moveSnake();               
    void spawnApple();
    void checkCollision(const UVec2& newHead);

    Field2 m_frame;
    Vec2 m_nextSnakeDir;
    Vec2 m_snakeDir;
    std::deque<UVec2> m_snakeBody;
    std::optional<UVec2> m_apple;

    GameState m_gameState;
    size_t m_currentScore;
    const size_t m_targetScore;
};