#include "bola.h"
#include "raymath.h"




void iniciabola(bola *b, float x, float y){
    b->pos.x=x;
    b->pos.y=y;
    b->vel.x=0;
    b->vel.y=0;
    b->raio=8;
    b->tacadas=0;
}
void desenhabola(bola *b){
    DrawCircleV(b->pos, b->raio, WHITE);
}
void atualizabola(bola *b, float dt){
    float fator = 1.0f - 2.0f * dt;
    if (fator < 0) fator = 0;
    b->vel = Vector2Scale(b->vel, fator);
    if (Vector2Length(b->vel)< 5.0f){
        b->vel.x = 0;
        b->vel.y = 0;
    }
    b->pos = Vector2Add(b->pos, Vector2Scale(b->vel, dt));
}