#include "mapa.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int **criamatriz(int linhas, int colunas){
    int **m = malloc(linhas * sizeof(int*));
    if (m == NULL) return NULL;

    for (int i = 0; i < linhas; i++){
        m[i] = malloc(colunas * sizeof(int));
        if (m[i] == NULL){
            for (int j = 0; j < i; j++) free(m[j]);
            free(m);
            return NULL;
        }
    }
    return m;
}

void liberamatriz(int **m, int linhas){
    if (m == NULL) return;
    for (int i = 0; i < linhas; i++) free(m[i]);
    free(m);
}

mapa* carregarmapa(const char *caminhoArquivo){
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo do mapa: %s\n", caminhoArquivo);
        return NULL;
    }
    mapa *m = malloc(sizeof(mapa));
    if (m == NULL){
        fclose(arquivo);
        return NULL;
    }
    if (fscanf(arquivo, "%d %d %d", &m->linha, &m->coluna, &m->par) != 3
    || m->linha <= 0 || m->coluna <= 0 || m->par <= 0){
        printf("Cabecalho invalido no mapa: %s\n", caminhoArquivo);
        free(m);
        fclose(arquivo);
        return NULL;
    }
    m->chao = criamatriz(m->linha, m->coluna);
    if (m->chao == NULL){
        free(m);
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < m->linha; i++){
        for (int j = 0; j < m->coluna; j++){
            int v;
            if (fscanf(arquivo, "%d", &v) != 1){
                printf("Mapa incompleto na linha %d, coluna %d\n", i, j);
                v = parede;
            } else if (v < grama || v > inicio){
                printf("Piso invalido (%d) na linha %d, coluna %d\n", v, i, j);
                v = parede;
            }
            m->chao[i][j] = v;
        }
    }
    fclose(arquivo);

    m->posini.x = TILE * 2;
    m->posini.y = TILE * 2;
    m->posbur.x = TILE * 5;
    m->posbur.y = TILE * 5;

    int nb = 0, si = 0, sj = 0;

    for (int i = 0; i < m->linha; i++){
        for (int j = 0; j < m->coluna; j++){
            if (m->chao[i][j] == inicio){
                m->posini.x = j * TILE + TILE / 2;
                m->posini.y = i * TILE + TILE / 2;
                m->chao[i][j] = grama;
            }
            if (m->chao[i][j] == buraco){
                nb++;
                si += i;
                sj += j;
            }
        }
    }

    if (nb > 0){
        int bi = si / nb, bj = sj / nb;
        for (int i = 0; i < m->linha; i++)
            for (int j = 0; j < m->coluna; j++)
                if (m->chao[i][j] == buraco) m->chao[i][j] = grama;
        m->chao[bi][bj] = buraco;
        m->posbur.x = bj * TILE + TILE / 2;
        m->posbur.y = bi * TILE + TILE / 2;
    }
    return m;
}

void liberarmapa(mapa *m){
    if (m == NULL) return;
    liberamatriz(m->chao, m->linha);
    free(m);
}

static void desenhartile(int piso, int i, int j, double tempo){
    const int t = TILE;
    const float k = t / 40.0f;
    int x = j * t, y = i * t;

    switch (piso){
    case grama:
    case buraco: {
        Color c = (i + j) % 2 == 0 ? (Color){96, 180, 90, 255} : (Color){88, 168, 82, 255};
        DrawRectangle(x, y, t, t, c);
        break;
    }
    case areia:
        DrawRectangle(x, y, t, t, (Color){232, 210, 150, 255});
        DrawCircleV((Vector2){x + ((i*13 + j*7) % 30 + 5) * k, y + ((i*5 + j*11) % 30 + 5) * k}, 1.5f * k + 0.5f, (Color){200, 175, 120, 255});
        break;
    case gelo:
        DrawRectangle(x, y, t, t, (Color){190, 230, 245, 255});
        DrawLineEx((Vector2){x + 8 * k, y + t - 8 * k}, (Vector2){x + t - 8 * k, y + 8 * k}, 1.0f, (Color){255, 255, 255, 200});
        break;
    case barro:
        DrawRectangle(x, y, t, t, (Color){120, 85, 55, 255});
        DrawCircleV((Vector2){x + ((i*9 + j*13) % 28 + 6) * k, y + ((i*17 + j*5) % 28 + 6) * k}, 4 * k, (Color){100, 70, 45, 255});
        break;
    case parede:
        DrawRectangle(x, y, t, t, (Color){70, 70, 85, 255});
        DrawRectangle(x, y, t, t / 8, (Color){120, 120, 140, 255});
        DrawRectangleLinesEx((Rectangle){x, y, t, t}, 1.0f, (Color){50, 50, 60, 255});
        break;
    case agua:
        DrawRectangle(x, y, t, t, (Color){50, 120, 200, 255});
        DrawRectangleRec((Rectangle){x + 8 * k + sinf(tempo * 2 + i + j) * 6 * k, y + 18 * k, 14 * k, 3 * k},
                         (Color){160, 210, 255, 255});
        break;
    }

    if (piso == buraco){
        int cx = x + t/2, cy = y + t/2;
        DrawCircle(cx, cy, t * 0.45f, (Color){20, 20, 20, 255});
        DrawRectangle(cx, cy - 2 * t, 2, 2 * t, WHITE);
        DrawTriangle((Vector2){cx + 2, cy - 2 * t}, (Vector2){cx + 2, cy - 1.4f * t}, (Vector2){cx + t * 0.6f, cy - 1.7f * t}, RED);
    }
}

void desenharmapa(const mapa *m){
    if (m == NULL || m->chao == NULL) return;
    double tempo = GetTime();
    for (int i = 0; i < m->linha; i++)
        for (int j = 0; j < m->coluna; j++)
            desenhartile(m->chao[i][j], i, j, tempo);
}

int pisoaqui(const mapa *m, float x, float y){
    if (m == NULL || x < 0 || y < 0) return parede;
    int j = (int)(x / TILE);
    int i = (int)(y / TILE);
    if (i >= m->linha || j >= m->coluna) return parede;
    return m->chao[i][j];
}
