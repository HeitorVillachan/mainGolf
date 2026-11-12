#include "raylib.h"
#include "mapa.h"

int main(void){
    InitWindow(800, 480, "Golf");
    SetTargetFPS(60);

    mapa *m = carregarmapa("assets/mapas/mapa1.txt");

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLACK);
        desenharmapa(m, TILE);
        EndDrawing();
    }

    CloseWindow();
    liberarmapa(m);
    return 0;
}