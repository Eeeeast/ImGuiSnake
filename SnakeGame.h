#include <deque>
#include <map>
#include <functional>
#include "Field2.h"
#include "Vec2.h"

class SnakeGame
{
public:
    SnakeGame(unsigned char boardWidth, unsigned char boardHeight);

    enum GameState
    {
        Running,
        Paused,
        GameOver,
        GameWin
    };

    void ApplyFrameDelay() const;
    void ProcessUserInput();
    void Update();
    GameState GetGameState() const { return gameState; }

    size_t GetCurrentScore() const { return currentScore; }
    size_t GetTargetScore() const { return targetScore; }
    const Field2& GetFrame() const { return frame; }

private:
    void ChangeSnakeDir(Vec2 desiredDir);

    void RestartGame();
    void HandlePause();

    Field2 frame;
    Vec2 nextSnakeDir, snakeDir;
    std::deque<UVec2> snakeBody;
    std::optional<UVec2> apple;

    GameState gameState;
    size_t currentScore;
    const size_t targetScore;
};