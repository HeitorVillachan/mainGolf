#include "video.h"
#include <math.h>

bool telacheia(void){
    return IsWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
}

static void ajustajanela(const video *v){
    int mon = GetCurrentMonitor();
    float mw = GetMonitorWidth(mon) * 0.9f;
    float mh = GetMonitorHeight(mon) * 0.9f;
    float f = 1.0f;
    if (v->largura * f > mw) f = mw / v->largura;
    if (v->altura  * f > mh) f = mh / v->altura;

    int w = (int)(v->largura * f), h = (int)(v->altura * f);
    Vector2 p = GetMonitorPosition(mon);
    SetWindowSize(w, h);
    SetWindowPosition((int)p.x + (GetMonitorWidth(mon) - w) / 2,
                      (int)p.y + (GetMonitorHeight(mon) - h) / 2);
}

static void alternartelacheia(video *v){
    ToggleBorderlessWindowed();
    if (telacheia()) HideCursor();
    else { ShowCursor(); ajustajanela(v); }
}

void iniciavideo(video *v, int largura, int altura, const char *titulo){
    v->largura = largura;
    v->altura = altura;
    v->escala = 1.0f;
    v->offset = (Vector2){0, 0};

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(largura, altura, titulo);

    if (!telacheia()) alternartelacheia(v);
}

void definirresolucao(video *v, int largura, int altura){
    if (v->largura == largura && v->altura == altura) return;
    v->largura = largura;
    v->altura = altura;
    if (!telacheia()) ajustajanela(v);
}

void atualizavideo(video *v){
    if (IsKeyPressed(KEY_F11)) alternartelacheia(v);

    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    float ex = sw / v->largura;
    float ey = sh / v->altura;
    v->escala = ex < ey ? ex : ey;
    v->offset.x = floorf((sw - v->largura * v->escala) / 2);
    v->offset.y = floorf((sh - v->altura  * v->escala) / 2);

    SetMouseOffset(-(int)v->offset.x, -(int)v->offset.y);
    SetMouseScale(1.0f / v->escala, 1.0f / v->escala);
}

void comecardesenho(video *v){
    BeginDrawing();
    ClearBackground(BLACK); 

    BeginScissorMode((int)v->offset.x, (int)v->offset.y,
                     (int)ceilf(v->largura * v->escala),
                     (int)ceilf(v->altura * v->escala));

    Camera2D cam = {0};
    cam.offset = v->offset;
    cam.zoom = v->escala;
    BeginMode2D(cam);
}

void terminardesenho(void){
    EndMode2D();
    EndScissorMode();
    EndDrawing();
}
