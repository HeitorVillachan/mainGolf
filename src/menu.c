#include "menu.h"

static const Color FUNDO   = {21, 26, 23, 255};
static const Color CLARO   = {232, 237, 230, 255};
static const Color PONT    = {110, 123, 112, 255};
static const Color VERDE   = {143, 203, 106, 255};
static const Color APAGADO = {124, 138, 128, 255};
static const Color FRACO   = {76, 87, 80, 255};
static const Color BARRA   = {34, 48, 42, 255};
static const Color LINHA   = {42, 51, 45, 255};

static float texto(Font f, const char *s, float x, float y, float tam, Color c){
    DrawTextEx(f, s, (Vector2){x, y}, tam, 0, c);
    return x + MeasureTextEx(f, s, tam, 0).x;
}

void iniciamenu(menu *mn){
    mn->opcao = 0;
    mn->fonte = GetFontDefault();
    mn->fonteok = false;
    if (FileExists("assets/fonts/JetBrainsMono-Regular.ttf")){
        mn->fonte = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf", 56, NULL, 0);
        SetTextureFilter(mn->fonte.texture, TEXTURE_FILTER_BILINEAR);
        mn->fonteok = true;
    }
}

void liberamenu(menu *mn){
    if (mn->fonteok) UnloadFont(mn->fonte);
}

int atualizamenu(menu *mn){
    if (IsKeyPressed(KEY_DOWN)) mn->opcao = (mn->opcao + 1) % 3;
    if (IsKeyPressed(KEY_UP))   mn->opcao = (mn->opcao + 2) % 3;
    if (IsKeyPressed(KEY_ENTER)) return mn->opcao;
    return OPC_NENHUMA;
}

void desenhamenu(menu *mn){
    Font f = mn->fonte;
    const char *nomes[3] = { "jogar", "ranking", "sair" };

    ClearBackground(FUNDO);

    float x = 60;
    x = texto(f, "main", x, 50, 56, CLARO);
    x = texto(f, "(", x, 50, 56, PONT);
    x = texto(f, "Golf", x, 50, 56, VERDE);
    texto(f, ")", x, 50, 56, PONT);
    DrawRectangle(60, 124, 680, 2, LINHA);

    int ybarra = 171 + (mn->opcao + 1) * 46 - 13;
    DrawRectangle(60, ybarra, 400, 40, BARRA);
    DrawRectangle(60, ybarra, 4, 40, VERDE);

    for (int n = 1; n <= 5; n++){
        const char *s = TextFormat("%d", n);
        bool sel = (n >= 2 && n <= 4 && mn->opcao == n - 2);
        float w = MeasureTextEx(f, s, 18, 0).x;
        texto(f, s, 96 - w, 171 + (n - 1) * 46 + 4, 18, sel ? VERDE : FRACO);
    }

    x = 120;
    x = texto(f, "int ", x, 171, 24, VERDE);
    x = texto(f, "main", x, 171, 24, CLARO);
    texto(f, "(void){", x, 171, 24, PONT);

    for (int i = 0; i < 3; i++){
        float y = 171 + (i + 1) * 46;
        bool sel = (i == mn->opcao);
        float xi = texto(f, nomes[i], 178, y, 24, sel ? CLARO : APAGADO);
        xi = texto(f, "();", xi, y, 24, sel ? PONT : FRACO);
        if (sel && (int)(GetTime() * 2) % 2 == 0)
            DrawRectangle((int)xi + 4, (int)y - 3, 13, 28, VERDE);
    }

    texto(f, "}", 120, 171 + 4 * 46, 24, PONT);
    texto(f, "// cima/baixo escolhe, ENTER confirma", 60, 422, 14, FRACO);
}

bool atualizaranking(void){
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_BACKSPACE);
}

void desenharanking(menu *mn, no *rank){
    Font f = mn->fonte;

    ClearBackground(FUNDO);

    float x = 60;
    x = texto(f, "ranking", x, 50, 56, CLARO);
    texto(f, "();", x, 50, 56, PONT);
    DrawRectangle(60, 124, 680, 2, LINHA);

    if (rank == NULL){
        texto(f, "// nenhuma pontuacao ainda", 120, 175, 22, FRACO);
    } else {
        int i = 0;
        for (no *p = rank; p != NULL && i < 5; p = p->prox, i++){
            float y = 171 + i * 46;
            bool topo = (i == 0);

            const char *s = TextFormat("%d", i + 1);
            float w = MeasureTextEx(f, s, 18, 0).x;
            texto(f, s, 96 - w, y + 4, 18, topo ? VERDE : FRACO);

            texto(f, p->nome, 120, y, 24, topo ? CLARO : APAGADO);

            const char *pts = TextFormat("%d", p->pontos);
            w = MeasureTextEx(f, pts, 24, 0).x;
            texto(f, pts, 520 - w, y, 24, topo ? VERDE : APAGADO);
        }
    }

    texto(f, "// ENTER volta", 60, 422, 14, FRACO);
}