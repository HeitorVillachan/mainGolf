#include "raylib.h"
#include "mapa.h"
#include "bola.h"

int main(void){
    InitWindow(800, 480, "Golf");
    SetTargetFPS(60);

    mapa *m = carregarmapa("assets/mapas/mapa1.txt");
    if (m == NULL){
        CloseWindow();
        return 1;
    }
    SetWindowSize(m->coluna * TILE, m->linha * TILE);

    bola b;
    iniciabola(&b, m->posini.x, m->posini.y);

    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;   // evita saltos se o jogo travar

        if (IsKeyPressed(KEY_R)) iniciabola(&b, m->posini.x, m->posini.y);

        tratarentrada(&b);
        atualizabola(&b, m, dt);

        BeginDrawing();
        ClearBackground(BLACK);
        desenharmapa(m, TILE);
        desenhamira(&b);
        desenhabola(&b);
        DrawText(TextFormat("Tacadas: %d", b.tacadas), 10, 10, 20, WHITE);
        DrawText("<- -> mira | cima/baixo forca | ESPACO tacar | SHIFT fino | R reiniciar",
                 10, 35, 16, WHITE);
        if (b.noburaco) DrawText("No buraco! Aperte R", 10, 60, 20, YELLOW);
        EndDrawing();
    }

    liberarmapa(m);
    CloseWindow();
    return 0;
}