#ifndef MAPA_H
#define MAPA_H
#include "raylib.h"
#define TILE 40
int **criamatriz(int linhas, int colunas);

typedef enum{ //nesse enum ele enumera sozinho, comecando do 0
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
void liberamatriz(int **m, int linhas);
int lemapa(mapa *m, char *arq);
void liberamapa(mapa *m);
int pisoaqui(mapa *m, float x, float y);
void desenhamapa(mapa *m);

#endif