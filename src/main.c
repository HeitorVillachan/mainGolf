#include "raylib.h"
#include "mapa.h"
#include "bola.h"
#include <stdio.h>
#include <stdlib.h>

static mapa* carregarfase(int fase){
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "assets/mapas/mapa%d.txt", fase);
    if (!FileExists(caminho)) return NULL;
    return carregarmapa(caminho);
}

int main(void){
    InitWindow(800, 480, "Golf");
    SetTargetFPS(60);

    int fase = 1;
    int totaltacadas = 0;
    bool fim = false;

    mapa *m = carregarfase(fase);
    if (m == NULL){
        CloseWindow();
        return 1;
    }
    SetWindowSize(m->coluna * TILE, m->linha * TILE);

    bola b;
    iniciabola(&b, m->posini.x, m->posini.y);

    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;

        if (!fim && IsKeyPressed(KEY_R)) iniciabola(&b, m->posini.x, m->posini.y);

        if (!fim && b.noburaco && IsKeyPressed(KEY_ENTER)){
            totaltacadas += b.tacadas;
            mapa *prox = carregarfase(fase + 1);
            if (prox == NULL){
                fim = true;               
            } else {
                liberarmapa(m);
                m = prox;
                fase++;
                SetWindowSize(m->coluna * TILE, m->linha * TILE);
                iniciabola(&b, m->posini.x, m->posini.y);
            }
        }

        if (!fim){
            tratarentrada(&b);
            atualizabola(&b, m, dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        desenharmapa(m, TILE);
        if (!fim){
            desenhamira(&b);
            desenhabola(&b);
            DrawText(TextFormat("Fase %d | Tacadas: %d", fase, b.tacadas), 10, 10, 20, WHITE);
            DrawText("<- -> mira | cima/baixo forca | ESPACO tacar | SHIFT fino | R reiniciar",
                     10, 35, 16, WHITE);
            if (b.noburaco) DrawText("No buraco! ENTER para continuar", 10, 60, 20, YELLOW);
        } else {
            DrawText("FIM DE JOGO!", 10, 10, 30, YELLOW);
            DrawText(TextFormat("Total de tacadas: %d", totaltacadas), 10, 50, 20, WHITE);
        }
        EndDrawing();
    }

    liberarmapa(m);
    CloseWindow();
    return 0;
}