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

    for (int i = 0; i < m->linha; i++){
        for (int j = 0; j < m->coluna; j++){
            if (m->chao[i][j] == inicio){
                m->posini.x = j * TILE + TILE / 2;
                m->posini.y = i * TILE + TILE / 2;
                m->chao[i][j] = grama;
            }
            if (m->chao[i][j] == buraco){
                m->posbur.x = j * TILE + TILE / 2;
                m->posbur.y = i * TILE + TILE / 2;
            }
        }
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
        DrawCircle(x + (i*13 + j*7) % 30 + 5, y + (i*5 + j*11) % 30 + 5, 1.5f, (Color){200, 175, 120, 255});
        DrawCircle(x + (i*7 + j*17) % 30 + 5, y + (i*11 + j*3) % 30 + 5, 1.5f, (Color){200, 175, 120, 255});
        break;
    case gelo:
        DrawRectangle(x, y, t, t, (Color){190, 230, 245, 255});
        DrawLineEx((Vector2){x + 8, y + t - 8}, (Vector2){x + t - 8, y + 8}, 1.0f, (Color){255, 255, 255, 200});
        break;
    case barro:
        DrawRectangle(x, y, t, t, (Color){120, 85, 55, 255});
        DrawCircle(x + (i*9 + j*13) % 28 + 6, y + (i*17 + j*5) % 28 + 6, 4, (Color){100, 70, 45, 255});
        break;
    case parede:
        DrawRectangle(x, y, t, t, (Color){70, 70, 85, 255});
        DrawRectangle(x, y, t, 5, (Color){120, 120, 140, 255});
        DrawRectangleLinesEx((Rectangle){x, y, t, t}, 1.0f, (Color){50, 50, 60, 255});
        break;
    case agua:
        DrawRectangle(x, y, t, t, (Color){50, 120, 200, 255});
        DrawRectangleRec((Rectangle){x + 8 + sinf(tempo * 2 + i + j) * 6, y + 18, 14, 3},
                         (Color){160, 210, 255, 255});
        break;
    }

    if (piso == buraco){
        int cx = x + t/2, cy = y + t/2;
        DrawCircle(cx, cy, 11, (Color){20, 20, 20, 255});
        DrawRectangle(cx, cy - 30, 2, 30, WHITE);
        DrawTriangle((Vector2){cx + 2, cy - 30}, (Vector2){cx + 2, cy - 20}, (Vector2){cx + 16, cy - 25}, RED);
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
