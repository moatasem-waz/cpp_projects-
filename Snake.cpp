#include "raylib.h"
#include <deque>
#include <cstdlib>

constexpr int CELL_SIZE = 25;
constexpr int COLS = 24;
constexpr int ROWS = 24;
constexpr int HUD_HEIGHT = 50;
constexpr int SCREEN_WIDTH = COLS * CELL_SIZE;
constexpr int SCREEN_HEIGHT = ROWS * CELL_SIZE + HUD_HEIGHT;

struct Cell {
    int x, y;
};

bool SameCell(Cell a, Cell b) {
    return a.x == b.x && a.y == b.y;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Snake - C++");
    SetTargetFPS(60);

    const Color background = { 25, 30, 38, 255 };
    const Color snakeColor = { 80, 210, 120, 255 };
    const Color foodColor = { 240, 80, 80, 255 };

    std::deque<Cell> snake = { {8, 10}, {7, 10}, {6, 10} };
    Cell direction = {1, 0};
    Cell nextDirection = direction;
    Cell food = {15, 10};

    int score = 0;
    bool gameOver = false;
    float timer = 0.0f;
    const float moveInterval = 0.13f;

    auto placeFood = [&]() {
        bool onSnake;
        do {
            onSnake = false;
            food = { GetRandomValue(0, COLS - 1),
                     GetRandomValue(0, ROWS - 1) };

            for (const Cell& part : snake) {
                if (SameCell(part, food)) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    };

    while (!WindowShouldClose()) {
        if (!gameOver) {
            if (IsKeyPressed(KEY_UP) && direction.y != 1)
                nextDirection = {0, -1};
            else if (IsKeyPressed(KEY_DOWN) && direction.y != -1)
                nextDirection = {0, 1};
            else if (IsKeyPressed(KEY_LEFT) && direction.x != 1)
                nextDirection = {-1, 0};
            else if (IsKeyPressed(KEY_RIGHT) && direction.x != -1)
                nextDirection = {1, 0};

            timer += GetFrameTime();

            if (timer >= moveInterval) {
                timer = 0.0f;
                direction = nextDirection;

                Cell head = {
                    snake.front().x + direction.x,
                    snake.front().y + direction.y
                };

                if (head.x < 0 || head.x >= COLS ||
                    head.y < 0 || head.y >= ROWS) {
                    gameOver = true;
                } else {
                    bool ateFood = SameCell(head, food);

                    // عند عدم أكل الطعام، الذيل سيتحرك؛ لذا لا نعدّ آخر خانة اصطدامًا.
                    for (size_t i = 0; i < snake.size(); ++i) {
                        if (!ateFood && i == snake.size() - 1)
                            break;

                        if (SameCell(head, snake[i])) {
                            gameOver = true;
                            break;
                        }
                    }

                    if (!gameOver) {
                        snake.push_front(head);

                        if (ateFood) {
                            ++score;
                            placeFood();
                        } else {
                            snake.pop_back();
                        }
                    }
                }
            }
        } else if (IsKeyPressed(KEY_R)) {
            snake = { {8, 10}, {7, 10}, {6, 10} };
            direction = {1, 0};
            nextDirection = direction;
            score = 0;
            timer = 0.0f;
            gameOver = false;
            placeFood();
        }

        BeginDrawing();
        ClearBackground(background);

        DrawText(TextFormat("Score: %i", score), 12, 12, 25, RAYWHITE);
        DrawText("Arrow keys: move", SCREEN_WIDTH - 205, 16, 18, LIGHTGRAY);

        DrawRectangle(food.x * CELL_SIZE, HUD_HEIGHT + food.y * CELL_SIZE,
                      CELL_SIZE - 2, CELL_SIZE - 2, foodColor);

        for (size_t i = 0; i < snake.size(); ++i) {
            DrawRectangle(snake[i].x * CELL_SIZE,
                          HUD_HEIGHT + snake[i].y * CELL_SIZE,
                          CELL_SIZE - 2, CELL_SIZE - 2, snakeColor);
        }

        if (gameOver) {
            DrawRectangle(0, HUD_HEIGHT, SCREEN_WIDTH,
                          SCREEN_HEIGHT - HUD_HEIGHT,
                          Fade(BLACK, 0.65f));
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - 110,
                     SCREEN_HEIGHT / 2 - 35, 40, RED);
            DrawText("Press R to restart", SCREEN_WIDTH / 2 - 105,
                     SCREEN_HEIGHT / 2 + 15, 22, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
