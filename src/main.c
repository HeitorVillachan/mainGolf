#include "raylib.h"
#include "mapa.h"
#include "stdio.h"

int main(void) {
    InitWindow(800, 600, "Golf - Teste de Mapa");
    SetTargetFPS(60);

    // Carrega o arquivo criado dentro da pasta assets
    mapa *m = carregarmapa("assets/mapateste.txt");

    // Tamanho em pixels de cada bloco (80px * 5 blocos = 400px na tela)
    float tamanhoTile = 80.0f; 

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (m != NULL) {
                desenharmapa(m, tamanhoTile);
            } else {
                DrawText("Erro ao abrir assets/mapateste.txt", 20, 20, 20, RED);
            }

        EndDrawing();
    }

    liberarmapa(m);
    CloseWindow();
    return 0;
}