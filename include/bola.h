#ifndef BOLA_H
#define BOLA_H
#include "raylib.h"

typedef struct bola{
    Vector2 pos, vel;
    float raio;
    int tacadas;
}bola;

void iniciabola(bola *b, float x, float y);


void atualizabola(bola *b, float dt);
void desenhabola(bola *b);

#endif