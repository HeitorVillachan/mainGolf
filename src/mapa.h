#ifndef MAPA_H
#define MAPA_H
#include "raylib.h"
#define TILE 40

typedef enum{ // o enum numera sozinho, na ordem
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
    int linha, coluna, **chao;
    Vector2 posini, posbur;
}mapa;

int **criamatriz(int linhas, int colunas);
void liberamatriz(int **m, int linhas);
mapa* carregarmapa(const char *caminhoArquivo);
void liberarmapa(mapa *m);
int pisoaqui(mapa *m, float x, float y);
void desenharmapa(mapa *m, float tamanhotile);

#endif