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
    int tacadas, quedas;
    float angulo;
    float forca;
    bool mirando;
    bool noburaco;
}bola;

void iniciabola(bola *b, float x, float y);
void tratarentrada(bola *b, float dt);
void atualizabola(bola *b, const mapa *m, float dt);
void desenhabola(const bola *b);
void desenhamira(const bola *b, int largura, int altura);
bool bolaparada(const bola *b);
#endif
