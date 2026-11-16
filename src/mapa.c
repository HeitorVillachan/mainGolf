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

    // cada número do arquivo corresponde a um valor do enum pisos
    for (int i = 0; i < m->linha; i++){
        for (int j = 0; j < m->coluna; j++){
            if (fscanf(arquivo, "%d", &m->chao[i][j]) != 1){
                printf("Mapa incompleto na linha %d, coluna %d\n", i, j);
                m->chao[i][j] = parede;
            }
        }
    }
    fclose(arquivo);

    // valores padrão caso o mapa não tenha início ou buraco
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

void desenharmapa(mapa *m, float tamanhotile){
    if (m == NULL || m->chao == NULL) return;
    int t = tamanhotile;
    for (int i = 0; i < m->linha; i++){
        for (int j = 0; j < m->coluna; j++){
            int x = j * t, y = i * t;
            int p = m->chao[i][j];
            if (p == grama || p == buraco){
                Color c = (i + j) % 2 == 0 ? (Color){96, 180, 90, 255} : (Color){88, 168, 82, 255};
                DrawRectangle(x, y, t, t, c);}
            else if (p == areia){
                DrawRectangle(x, y, t, t, (Color){232, 210, 150, 255});
                DrawCircle(x + (i*13 + j*7) % 30 + 5, y + (i*5 + j*11) % 30 + 5, 1.5, (Color){200, 175, 120, 255});
                DrawCircle(x + (i*7 + j*17) % 30 + 5, y + (i*11 + j*3) % 30 + 5, 1.5, (Color){200, 175, 120, 255});}
            else if (p == gelo){
                DrawRectangle(x, y, t, t, (Color){190, 230, 245, 255});
                DrawLine(x + 8, y + t - 8, x + t - 8, y + 8, (Color){255, 255, 255, 200});}
            else if (p == barro){
                DrawRectangle(x, y, t, t, (Color){120, 85, 55, 255});
                DrawCircle(x + (i*9 + j*13) % 28 + 6, y + (i*17 + j*5) % 28 + 6, 4, (Color){100, 70, 45, 255});}
            else if (p == parede){
                DrawRectangle(x, y, t, t, (Color){70, 70, 85, 255});
                DrawRectangle(x, y, t, 5, (Color){120, 120, 140, 255});
                DrawRectangleLines(x, y, t, t, (Color){50, 50, 60, 255});}
            else if (p == agua){
                DrawRectangle(x, y, t, t, (Color){50, 120, 200, 255});
                DrawRectangle(x + 8 + (int)(sinf(GetTime()*2 + i + j) * 6), y + 18, 14, 3, (Color){160, 210, 255, 255});}
            if (p == buraco){
                int cx = x + t/2, cy = y + t/2;
                DrawCircle(cx, cy, 11, (Color){20, 20, 20, 255});
                DrawRectangle(cx, cy - 30, 2, 30, WHITE);
                DrawTriangle((Vector2){cx + 2, cy - 30}, (Vector2){cx + 2, cy - 20}, (Vector2){cx + 16, cy - 25}, RED);
            }}}}

int pisoaqui(mapa *m, float x, float y){
    if (m == NULL || x < 0 || y < 0) return parede;
    int j = x / TILE;
    int i = y / TILE;
    if (i >= m->linha || j >= m->coluna) return parede;
    return m->chao[i][j];
}