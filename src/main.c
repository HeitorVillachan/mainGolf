#include "raylib.h"
#include "mapa.h"

int main(void){
    InitWindow(800, 480, "Golf");
    SetTargetFPS(60);

    mapa *m = carregarmapa("assets/mapas/mapa1.txt");

<<<<<<< Updated upstream
    while (!WindowShouldClose()){
=======
    // Tamanho em pixels de cada bloco (80px * 5 blocos = 400px na tela)
    float tamanhoTile = 5.0f; 

    while (!WindowShouldClose()) {
>>>>>>> Stashed changes
        BeginDrawing();
        ClearBackground(BLACK);
        desenharmapa(m, TILE);
        EndDrawing();
    }

    CloseWindow();
    liberarmapa(m);
    return 0;
}