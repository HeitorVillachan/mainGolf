#include "bola.h"
#include "raymath.h"
void iniciabola(bola *b, float x, float y){
    b->pos.x=x;
    b->pos.y=y;
    b->ultima = b->pos;
    b->vel.x=0;
    b->vel.y=0;
    b->raio=8;
    b->tacadas=0;
}
float atrito(int piso){
    if (piso == areia) return 2.5;
    if (piso == gelo) return 0.5;
    if (piso == barro) return 4.5;
    return 1.5;
}
void desenhabola(bola *b){
    DrawCircle((int)b->pos.x +2, (int)b->pos.y +3, b->raio, (Color){0, 0, 0, 70});
    DrawCircleV(b->pos,b->raio, WHITE);
    DrawCircleLines((int)b->pos.x, (int)b->pos.y, b->raio, LIGHTGRAY);
}



void atualizabola(bola *b, mapa *m, float dt){

    int piso = pisoaqui(m, b->pos.x, b->pos.y);
    float f = 1 - atrito(piso) * dt;
    if (f < 0) f = 0;
    b->vel = Vector2Scale(b->vel, f);
    if (Vector2Length(b->vel) < 8){
        b->vel.x = 0;//quando ta cm pouca força
        b->vel.y = 0;
    }
    float nx = b->pos.x + b->vel.x* dt;
    float lado = b->raio;
    if (b->vel.x < 0) lado = -lado;
    if (pisoaqui(m, nx + lado, b->pos.y) == parede) b->vel.x = -b->vel.x * 0.8;
    else b->pos.x = nx;
    float ny = b->pos.y + b->vel.y * dt;
    lado = b->raio;
    if (b->vel.y < 0) lado = -lado;
    if (pisoaqui(m, b->pos.x, ny + lado) == parede) b->vel.y = -b->vel.y * 0.8;
    else b->pos.y = ny;
    if (pisoaqui(m, b->pos.x, b->pos.y) == agua){
        b->pos = b->ultima;
        b->vel.x = 0;
        b->vel.y = 0;
        b->tacadas++;
    }
    
}

