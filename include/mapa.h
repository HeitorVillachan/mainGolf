#ifndef MAPA_H
#define MAPA_H
#include "raylib.h"
#define TILE 20

typedef enum{
    grama,
    areia,
    gelo,
    barro,
    parede,
    agua,
    buraco,
    inicio
}pisos;

typedef struct mapa{
    int linha, coluna, par, **chao;
    Vector2 posini, posbur;
}mapa;

int **criamatriz(int linhas, int colunas);
void liberamatriz(int **m, int linhas);
mapa* carregarmapa(const char *caminhoArquivo);
void liberarmapa(mapa *m);
int pisoaqui(const mapa *m, float x, float y);
void desenharmapa(const mapa *m);

#endif
