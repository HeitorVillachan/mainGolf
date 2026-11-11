#ifndef BOLA_H
#define BOLA_H

#include "mapa.h"
#include "raylib.h"


typedef struct bola{
    Vector2 pos, vel,ultima;
    float raio;
    int tacadas;
}bola;

void iniciabola(bola *b, float x, float y);


void atualizabola(bola *b, mapa *m, float dt);
void desenhabola(bola *b);

#endif