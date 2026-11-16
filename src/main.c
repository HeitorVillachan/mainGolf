#include "raylib.h"
#include "mapa.h"
#include "bola.h"
#include "pontos.h"
#include "ranking.h"
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
    placar pl;
    iniciaplacar(&pl);
    bool pontuada = false;

    mapa *m = carregarfase(fase);
    if (m == NULL){
        CloseWindow();
        return 1;
    }
    SetWindowSize(m->coluna * TILE, m->linha * TILE);

    bola b;
    iniciabola(&b, m->posini.x, m->posini.y);

    char nome[20] = "";
    int tamnome = 0;
    bool salvo = false;
    no *rank = NULL;

    while (!WindowShouldClose()){
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;

        if (!fim && !pontuada && IsKeyPressed(KEY_R)){
            int t = b.tacadas, q = b.quedas;
            iniciabola(&b, m->posini.x, m->posini.y);
            b.tacadas = t + 1;
            b.quedas = q;
        }

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
                pontuada = false;
            }
        }

        if (!fim){
            if (fim && !salvo){
                int c = GetCharPressed();
                while (c > 0){
                    if (c > 32 && c <= 126 && tamnome < 19){
                        nome[tamnome++] = (char)c;
                        nome[tamnome] = '\0';
                    }
                    c = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE) && tamnome > 0)
                    nome[--tamnome] = '\0';
                    
                if (IsKeyPressed(KEY_ENTER) && tamnome > 0){
                rank = carregarranking("scores.txt");
                rank = inserirordenado(rank, nome, pl.total);
                salvarranking(rank, "scores.txt");
                salvo = true;
                }
            }


            tratarentrada(&b);
            atualizabola(&b, m, dt);

            if (b.noburaco && !pontuada){
                pontuarfase(&pl, b.tacadas, m->par, b.quedas);
                pontuada = true;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        desenharmapa(m, TILE);
        if (!fim){
            desenhamira(&b);
            desenhabola(&b);
            DrawText(TextFormat("Fase %d | Par %d | Tacadas: %d | Pontos: %d",
                fase, m->par, b.tacadas, pl.total), 10, 10, 20, WHITE);
            DrawText("<- -> mira | cima/baixo forca | ESPACO tacar | SHIFT fino | R reiniciar",
                     10, 35, 16, WHITE);
            if (b.noburaco)
                DrawText(TextFormat("No buraco! +%d (x%d)  ENTER para continuar",
                         pl.ultimafase, pl.multiplicador), 10, 60, 20, YELLOW);
        } else {
            DrawText("FIM DE JOGO!", 10, 10, 30, YELLOW);
            DrawText(TextFormat("Total de tacadas: %d", totaltacadas), 10, 50, 20, WHITE);
            DrawText(TextFormat("Pontuacao final: %d", pl.total), 10, 80, 20, WHITE);

            if (!salvo){
                DrawText("Digite seu nome e ENTER:", 10, 115, 20, WHITE);
                DrawText(nome, 10, 145, 20, YELLOW);
            } else {
                DrawText("RANKING", 10, 115, 20, YELLOW);
                int y = 145, pos = 1;
                for (no *p = rank; p != NULL && pos <= 5; p = p->prox, pos++){
                    DrawText(TextFormat("%d. %s - %d", pos, p->nome, p->pontos), 10, y, 20, WHITE);
                    y += 25;
                }
            }
        }
        EndDrawing();
    }

    liberarmapa(m);
    liberarranking(rank);
    CloseWindow();
    return 0;
}