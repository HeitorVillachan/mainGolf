#include "mapa.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

int **criamatriz(int linhas, int colunas){
    int **m = malloc(linhas * sizeof(int*));
    if (m == NULL) return NULL;


    for(int i = 0; i<linhas;i++){
        m[i] = malloc(colunas * sizeof(int));

        if (m[i] == NULL) {
            for (int j = 0; j < i; j++) free(m[j]);
            free(m);
            return NULL;}
        }
    return m;}

void liberamatriz(int **m, int linhas) {
    if (m == NULL) return;
    for (int i = 0; i < linhas; i++) {
        free(m[i]);
    }
    free(m);
}
mapa* carregarmapa(const char *caminhoArquivo) {
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", caminhoArquivo);
        return NULL;
    }
}
mapa* carregarmapa(const char *caminhoArquivo) {
    FILE *arquivo = fopen(caminhoArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo do mapa: %s\n", caminhoArquivo);
        return NULL;
    }
    mapa *m = malloc(sizeof(mapa));
    if (m == NULL) {
        fclose(arquivo);
        return NULL;}
    if (fscanf(arquivo, "%d %d", &m->linha, &m->coluna) != 2) {
        free(m);
        fclose(arquivo);
        return NULL;}
    m->chao = criamatriz(m->linha, m->coluna);
    if (m->chao == NULL) {
        free(m);
        fclose(arquivo);
        return NULL;
    }
    for (int i = 0; i < m->linha; i++) { //vai preencher a matriz baseado no que tem no arquivo cada numero remete a uma struct dos pisos la 
        for (int j = 0; j < m->coluna; j++) {
            fscanf(arquivo, "%d", &m->chao[i][j]);
        }
    }
    fclose(arquivo);
    return m;
}
void liberarmapa(mapa *m) {
    if (m == NULL) return;
    liberamatriz(m->chao, m->linha);
    free(m);
}
