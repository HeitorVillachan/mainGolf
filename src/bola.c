#include "bola.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>

#define RESTITUICAO    0.8f           
#define PASSO_MAX      (1.0f/240.0f) 
#define VEL_MINIMA     8.0f
#define VEL_MAX_BURACO 250.0f        
#define FORCA_MIN      0.05f
#define FATOR_FINO     0.3f           

void iniciabola(bola *b, float x, float y){
    b->pos = (Vector2){x, y};
    b->ultima = b->pos;
    b->vel = (Vector2){0, 0};
    b->raio = 8;
    b->tacadas = 0;
    b->angulo = 0;
    b->forca = 0.5f;
    b->mirando = false;
    b->noburaco = false;
    b->quedas = 0;
}

static float atrito(int piso){
    if (piso == areia) return 2.5f;
    if (piso == gelo)  return 0.5f;
    if (piso == barro) return 4.5f;
    return 1.5f;
}

bool bolaparada(const bola *b){
    return b->vel.x == 0 && b->vel.y == 0;
}

void tratarentrada(bola *b, float dt){
    if (b->noburaco || !bolaparada(b)){
        b->mirando = false;
        return;
    }
    b->mirando = true;

    float fino = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? FATOR_FINO : 1.0f;

    if (IsKeyDown(KEY_LEFT))  b->angulo -= VEL_ANGULAR * fino * dt;
    if (IsKeyDown(KEY_RIGHT)) b->angulo += VEL_ANGULAR * fino * dt;
    if (IsKeyDown(KEY_UP))    b->forca  += VEL_FORCA * fino * dt;
    if (IsKeyDown(KEY_DOWN))  b->forca  -= VEL_FORCA * fino * dt;

    b->forca = Clamp(b->forca, FORCA_MIN, 1.0f);

    if (IsKeyPressed(KEY_SPACE)){
        b->ultima = b->pos;
        b->vel = Vector2Scale((Vector2){ cosf(b->angulo), sinf(b->angulo) },
                              b->forca * VEL_TACADA_MAX);
        b->tacadas++;
        b->mirando = false;
    }
}

static int pisotile(const mapa *m, int i, int j){
    if (i < 0 || j < 0 || i >= m->linha || j >= m->coluna) return parede;
    return m->chao[i][j];
}


static void colideparedes(bola *b, const mapa *m){
    int ci = (int)floorf(b->pos.y / TILE);
    int cj = (int)floorf(b->pos.x / TILE);

    for (int i = ci - 1; i <= ci + 1; i++){
        for (int j = cj - 1; j <= cj + 1; j++){
            if (pisotile(m, i, j) != parede) continue;

            float x0 = (float)(j * TILE), x1 = x0 + TILE;
            float y0 = (float)(i * TILE), y1 = y0 + TILE;

            float px = Clamp(b->pos.x, x0, x1);
            float py = Clamp(b->pos.y, y0, y1);
            Vector2 d = {b->pos.x - px, b->pos.y - py};
            float dist2 = d.x * d.x + d.y * d.y;
            if (dist2 >= b->raio * b->raio) continue;

            Vector2 n;
            if (dist2 > 0.0000001f){
                float dist = sqrtf(dist2);
                n = (Vector2){d.x / dist, d.y / dist};
                b->pos = Vector2Add(b->pos, Vector2Scale(n, b->raio - dist));
            } else {
                float esq = b->pos.x - x0, dir = x1 - b->pos.x;
                float topo = b->pos.y - y0, base = y1 - b->pos.y;
                float menor = esq;  n = (Vector2){-1, 0};
                if (dir  < menor){ menor = dir;  n = (Vector2){ 1, 0}; }
                if (topo < menor){ menor = topo; n = (Vector2){ 0,-1}; }
                if (base < menor){ menor = base; n = (Vector2){ 0, 1}; }
                b->pos = Vector2Add(b->pos, Vector2Scale(n, menor + b->raio));
            }

            float vn = Vector2DotProduct(b->vel, n);
            if (vn < 0)
                b->vel = Vector2Subtract(b->vel, Vector2Scale(n, (1 + RESTITUICAO) * vn));
        }
    }
}

void atualizabola(bola *b, const mapa *m, float dt){
    if (b->noburaco || m == NULL) return;

    int passos = (int)ceilf(dt / PASSO_MAX);
    if (passos < 1) passos = 1;
    float h = dt / passos;

    for (int s = 0; s < passos; s++){
        int piso = pisoaqui(m, b->pos.x, b->pos.y);

        float f = 1 - atrito(piso) * h;
        if (f < 0) f = 0;
        b->vel = Vector2Scale(b->vel, f);
        if (Vector2Length(b->vel) < VEL_MINIMA) b->vel = (Vector2){0, 0};

        b->pos = Vector2Add(b->pos, Vector2Scale(b->vel, h));
        colideparedes(b, m);

        piso = pisoaqui(m, b->pos.x, b->pos.y);
        if (piso == agua){
            b->pos = b->ultima;
            b->vel = (Vector2){0, 0};
            b->tacadas++;
            b->quedas++;
            return;
        }
        if (piso == buraco){
            float d = Vector2Distance(b->pos, m->posbur);
            if (d < TILE * 0.3f && Vector2Length(b->vel) < VEL_MAX_BURACO){
                b->noburaco = true;
                b->pos = m->posbur;
                b->vel = (Vector2){0, 0};
                return;
            }
        }
    }
}

void desenhamira(const bola *b, int largura, int altura){
    if (!b->mirando) return;
    Vector2 dir = {cosf(b->angulo), sinf(b->angulo)};
    for (int k = 1; k <= 8; k++){
        float dist = k * (10 + b->forca * 14);
        DrawCircleV(Vector2Add(b->pos, Vector2Scale(dir, dist)), 3, WHITE);
    }

    int x = largura / 2 - 100;
    int y = altura - 28;
    DrawRectangle(x, y, 200, 16, DARKGRAY);
    DrawRectangle(x, y, (int)(200 * b->forca), 16,
                  (Color){(unsigned char)(80 + 175 * b->forca),
                          (unsigned char)(220 - 150 * b->forca), 60, 255});
    DrawRectangleLinesEx((Rectangle){(float)x, (float)y, 200, 16}, 1.0f, WHITE);
}
void desenhabola(const bola *b){
    DrawCircleV((Vector2){b->pos.x + 2, b->pos.y + 3}, b->raio, (Color){0, 0, 0, 70});
    DrawCircleV(b->pos, b->raio, WHITE);
    DrawRing(b->pos, b->raio - 1, b->raio, 0, 360, 32, LIGHTGRAY);
}
