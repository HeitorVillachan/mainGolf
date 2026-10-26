#include "raylib.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Raylib - Teste de Colisao nas Bordas");

    Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    Vector2 ballSpeed = { 5.0f, 4.0f };
    float ballRadius = 20.0f;
    Color ballColor = MAROON;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        if ((ballPosition.x >= (screenWidth - ballRadius)) || (ballPosition.x <= ballRadius)) {
            ballSpeed.x *= -1.0f;
            ballColor = (Color){ GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255 };
        }

        if ((ballPosition.y >= (screenHeight - ballRadius)) || (ballPosition.y <= ballRadius)) {
            ballSpeed.y *= -1.0f;
            ballColor = (Color){ GetRandomValue(50, 255), GetRandomValue(50, 255), GetRandomValue(50, 255), 255 };
        }

        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawCircleV(ballPosition, ballRadius, ballColor);
            DrawText("Raylib - Quicando nas bordas!", 10, 10, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}