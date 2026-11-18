#include "raylib.h"
#include "video.h"
#include "mapa.h"
#include "bola.h"
#include "pontos.h"
#include "ranking.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ_RANKING "scores.txt"
#define DT_MAX      0.05f
#define TOP_RANKING 5

typedef enum { TELA_MENU, TELA_JOGO, TELA_RANKING } tela;

typedef struct jogo{
    video vid;
    menu mn;
    tela atual;
    bool sair;

    mapa *m;
    bola b;
    placar pl;
    int fase, totaltacadas;
    bool fim, pontuada, salvo;

    char nome[TAM_NOME];
    int tamnome;
    no *rank;
}jogo;

static mapa *carregarfase(int fase){
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "assets/mapas/mapa%d.txt", fase);
    if (!FileExists(caminho)) return NULL;
    return carregarmapa(caminho);
}

static void entrarfase(jogo *g){
    definirresolucao(&g->vid, g->m->coluna * TILE, g->m->linha * TILE);
    iniciabola(&g->b, g->m->posini.x, g->m->posini.y);
    g->pontuada = false;
}

static void reiniciarbola(jogo *g){
    bola *b = &g->b;
    int t = b->tacadas, q = b->quedas;
    float ang = b->angulo, forca = b->forca;
    iniciabola(b, g->m->posini.x, g->m->posini.y);
    b->tacadas = t + 1;       
    b->quedas = q;
    b->angulo = ang;         
    b->forca = forca;
}

static void avancarfase(jogo *g){
    g->totaltacadas += g->b.tacadas;
    mapa *prox = carregarfase(g->fase + 1);
    if (prox == NULL){
        g->fim = true;
        while (GetCharPressed() > 0); 
        return;
    }
    liberarmapa(g->m);
    g->m = prox;
    g->fase++;
    entrarfase(g);
}

static void voltarmenu(jogo *g){
    mapa *novo = carregarfase(1);
    if (novo == NULL){
        g->sair = true;
        return;
    }
    liberarmapa(g->m);
    g->m = novo;
    g->fase = 1;
    g->totaltacadas = 0;
    iniciaplacar(&g->pl);
    g->fim = false;
    g->salvo = false;
    g->tamnome = 0;
    g->nome[0] = '\0';
    iniciabola(&g->b, g->m->posini.x, g->m->posini.y);
    g->pontuada = false;
    definirresolucao(&g->vid, LARGURA_MENU, ALTURA_MENU);
    g->atual = TELA_MENU;
}

static void atualizarmenu(jogo *g){
    int escolha = atualizamenu(&g->mn);
    if (escolha == OPC_JOGAR){
        g->atual = TELA_JOGO;
        definirresolucao(&g->vid, g->m->coluna * TILE, g->m->linha * TILE);
    } else if (escolha == OPC_RANKING){
        g->atual = TELA_RANKING;
    } else if (escolha == OPC_SAIR){
        g->sair = true;
    }
}

static void atualizarnome(jogo *g){
    int c;
    while ((c = GetCharPressed()) > 0){
        if (c > 32 && c <= 126 && g->tamnome < TAM_NOME - 1){
            g->nome[g->tamnome++] = (char)c;
            g->nome[g->tamnome] = '\0';
        }
    }
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)) && g->tamnome > 0)
        g->nome[--g->tamnome] = '\0';

    if (IsKeyPressed(KEY_ENTER) && g->tamnome > 0){
        g->rank = inserirordenado(g->rank, g->nome, g->pl.total);
        salvarranking(g->rank, ARQ_RANKING);
        g->salvo = true;
    }
}

static void atualizarjogada(jogo *g, float dt){
    bola *b = &g->b;

    if (!g->pontuada && b->tacadas > 0 && IsKeyPressed(KEY_R))
        reiniciarbola(g);

    if (b->noburaco && IsKeyPressed(KEY_ENTER)){
        avancarfase(g);
        if (g->fim) return;
        b = &g->b;
    }

    tratarentrada(b, dt);
    atualizabola(b, g->m, dt);

    if (b->noburaco && !g->pontuada){
        pontuarfase(&g->pl, b->tacadas, g->m->par, b->quedas);
        g->pontuada = true;
    }
}

static void atualizarpartida(jogo *g, float dt){
    if (!g->fim){
        atualizarjogada(g, dt);
    } else if (!g->salvo){
        atualizarnome(g);
    } else if (IsKeyPressed(KEY_ENTER)){
        voltarmenu(g);
    }
}

static void atualizar(jogo *g, float dt){
    switch (g->atual){
    case TELA_MENU:    atualizarmenu(g); break;
    case TELA_RANKING: if (atualizaranking()) g->atual = TELA_MENU; break;
    case TELA_JOGO:    atualizarpartida(g, dt); break;
    }
}

static void desenharfim(const jogo *g){
    DrawText("FIM DE JOGO!", 10, 10, 30, YELLOW);
    DrawText(TextFormat("Total de tacadas: %d", g->totaltacadas), 10, 50, 20, WHITE);
    DrawText(TextFormat("Pontuacao final: %d", g->pl.total), 10, 80, 20, WHITE);

    if (!g->salvo){
        DrawText("Digite seu nome e ENTER:", 10, 115, 20, WHITE);
        DrawText(g->nome, 10, 145, 20, YELLOW);
        return;
    }

    DrawText("RANKING", 10, 115, 20, YELLOW);
    DrawText("(ENTER volta ao menu)", 120, 118, 16, LIGHTGRAY);
    int y = 145, pos = 1;
    for (const no *p = g->rank; p != NULL && pos <= TOP_RANKING; p = p->prox, pos++){
        DrawText(TextFormat("%d. %s - %d", pos, p->nome, p->pontos), 10, y, 20, WHITE);
        y += 25;
    }
}

static void desenharjogo(const jogo *g){
    ClearBackground(BLACK);
    desenharmapa(g->m);

    if (g->fim){
        desenharfim(g);
        return;
    }

    desenhamira(&g->b, g->vid.largura, g->vid.altura);
    desenhabola(&g->b);
    DrawText(TextFormat("Fase %d | Par %d | Tacadas: %d | Pontos: %d",
        g->fase, g->m->par, g->b.tacadas, g->pl.total), 10, 10, 20, WHITE);
    DrawText("<- -> mira | cima/baixo forca | ESPACO tacar | SHIFT fino | R reiniciar | F11 tela cheia",
             10, 35, 16, WHITE);
    if (g->b.noburaco)
        DrawText(TextFormat("No buraco! +%d (x%d)  ENTER para continuar",
                 g->pl.ultimafase, g->pl.multiplicador), 10, 60, 20, YELLOW);
}

static void desenhar(jogo *g){
    comecardesenho(&g->vid);
    switch (g->atual){
    case TELA_MENU:    desenhamenu(&g->mn); break;
    case TELA_RANKING: desenharranking(&g->mn, g->rank); break;
    case TELA_JOGO:    desenharjogo(g); break;
    }
    terminardesenho();
}

static bool iniciajogo(jogo *g){
    memset(g, 0, sizeof(*g));

    iniciavideo(&g->vid, LARGURA_MENU, ALTURA_MENU, "main(Golf)");
    iniciamenu(&g->mn);

    g->atual = TELA_MENU;
    g->fase = 1;
    iniciaplacar(&g->pl);

    g->m = carregarfase(g->fase);
    if (g->m == NULL){
        TraceLog(LOG_ERROR, "Nao foi possivel carregar assets/mapas/mapa1.txt");
        liberamenu(&g->mn);
        CloseWindow();
        return false;
    }
    iniciabola(&g->b, g->m->posini.x, g->m->posini.y);
    g->rank = carregarranking(ARQ_RANKING);
    return true;
}

static void finalizajogo(jogo *g){
    liberarmapa(g->m);
    liberarranking(g->rank);
    liberamenu(&g->mn);
    CloseWindow();
}

int main(void){
    jogo g;
    if (!iniciajogo(&g)) return 1;

    while (!WindowShouldClose() && !g.sair){
        float dt = GetFrameTime();
        if (dt > DT_MAX) dt = DT_MAX;

        atualizavideo(&g.vid);
        atualizar(&g, dt);
        desenhar(&g);
    }

    finalizajogo(&g);
    return 0;
}
