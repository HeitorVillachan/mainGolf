#include "bola.h"
#include "raymath.h"
#include <math.h>
#include <stdlib.h>

#define RESTITUICAO 0.8f      // quanto de velocidade sobra após bater
#define PASSOS 4              // subpassos por frame (evita atravessar parede)
#define VEL_MINIMA 8.0f
#define VEL_MAX_BURACO 250.0f // rápido demais = passa por cima do buraco

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
}

float atrito(int piso){
    if (piso == areia) return 2.5f;
    if (piso == gelo)  return 0.5f;
    if (piso == barro) return 4.5f;
    return 1.5f;
}

bool bolaparada(bola *b){
    return b->vel.x == 0 && b->vel.y == 0;
}

void tratarentrada(bola *b){
    if (b->noburaco || !bolaparada(b)){
        b->mirando = false;
        return;
    }
    b->mirando = true;

    float dt = GetFrameTime();
    float fino = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? 0.3f : 1.0f;

    if (IsKeyDown(KEY_LEFT))  b->angulo -= VEL_ANGULAR * fino * dt;
    if (IsKeyDown(KEY_RIGHT)) b->angulo += VEL_ANGULAR * fino * dt;
    if (IsKeyDown(KEY_UP))    b->forca  += VEL_FORCA * fino * dt;
    if (IsKeyDown(KEY_DOWN))  b->forca  -= VEL_FORCA * fino * dt;

    if (b->forca > 1.0f)  b->forca = 1.0f;
    if (b->forca < 0.05f) b->forca = 0.05f;

    if (IsKeyPressed(KEY_SPACE)){
        b->ultima = b->pos;   // ponto seguro se cair na água
        b->vel = (Vector2){ cosf(b->angulo), sinf(b->angulo) };
        b->vel = Vector2Scale(b->vel, b->forca * VEL_TACADA_MAX);
        b->tacadas++;
        b->mirando = false;
    }
}

static int pisotile(mapa *m, int i, int j){
    if (i < 0 || j < 0 || i >= m->linha || j >= m->coluna) return parede;
    return m->chao[i][j];
}

// colisão círculo x quadrado, para cada parede vizinha
static void colideparedes(bola *b, mapa *m){
    int ci = (int)(b->pos.y / TILE);
    int cj = (int)(b->pos.x / TILE);

    for (int i = ci - 1; i <= ci + 1; i++){
        for (int j = cj - 1; j <= cj + 1; j++){
            if (pisotile(m, i, j) != parede) continue;

            // ponto do tile mais próximo do centro da bola
            float px = Clamp(b->pos.x, j * TILE, (j + 1) * TILE);
            float py = Clamp(b->pos.y, i * TILE, (i + 1) * TILE);
            Vector2 d = {b->pos.x - px, b->pos.y - py};
            float dist2 = d.x * d.x + d.y * d.y;
            if (dist2 >= b->raio * b->raio) continue;

            float dist = sqrtf(dist2);
            Vector2 n;
            if (dist > 0.0001f){
                n = (Vector2){d.x / dist, d.y / dist};
                float sobra = b->raio - dist;       // empurra pra fora
                b->pos.x += n.x * sobra;
                b->pos.y += n.y * sobra;
            } else {
                // centro caiu dentro do tile: volta contra a velocidade
                n = (Vector2){0, -1};
                if (Vector2Length(b->vel) > 0) n = Vector2Normalize(Vector2Negate(b->vel));
                b->pos = Vector2Add(b->pos, Vector2Scale(n, b->raio));
            }

            float vn = Vector2DotProduct(b->vel, n);
            if (vn < 0)  // reflete só se estiver indo contra a parede
                b->vel = Vector2Subtract(b->vel, Vector2Scale(n, (1 + RESTITUICAO) * vn));
        }
    }
}

void atualizabola(bola *b, mapa *m, float dt){
    if (b->noburaco || m == NULL) return;
    float h = dt / PASSOS;

    for (int s = 0; s < PASSOS; s++){
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
            b->tacadas++;            // penalidade
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
    }}
void desenhamira(bola *b){
    if (!b->mirando) return;
    Vector2 dir = {cosf(b->angulo), sinf(b->angulo)};
    for (int k = 1; k <= 8; k++){
        float dist = k * (10 + b->forca * 14);
        DrawCircleV(Vector2Add(b->pos, Vector2Scale(dir, dist)), 3, WHITE);
    }
    int x = GetScreenWidth()/2-100;
    int y = GetScreenHeight() -28;
    DrawRectangle(x, y, 200, 16, DARKGRAY);
    DrawRectangle(x, y, (int)(200 * b->forca), 16,
                  (Color){80 + 175 * b->forca, 220 - 150 * b->forca, 60, 255});
    DrawRectangleLines(x, y, 200, 16, WHITE);
}
void desenhabola(bola *b){
    DrawCircle((int)b->pos.x + 2, (int)b->pos.y + 3, b->raio, (Color){0, 0, 0, 70});   // sombra
    DrawCircleV(b->pos, b->raio, WHITE);
    DrawCircleLines((int)b->pos.x, (int)b->pos.y, b->raio, LIGHTGRAY);
}

