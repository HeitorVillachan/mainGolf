#ifndef BOLA_H
#define BOLA_H
#include "raylib.h"
#include "mapa.h"

#define VEL_TACADA_MAX 600.0f
#define VEL_ANGULAR    2.5f
#define VEL_FORCA      0.8f

typedef struct bola{
    Vector2 pos, vel, ultima;
    float raio;
    int tacadas;
    float angulo;
    float forca;
    bool mirando;
    bool noburaco;
}bola;

void iniciabola(bola *b, float x, float y);
void tratarentrada(bola *b);
void atualizabola(bola *b, mapa *m, float dt);
void desenhabola(bola *b);
void desenhamira(bola *b);
bool bolaparada(bola *b);
#endif