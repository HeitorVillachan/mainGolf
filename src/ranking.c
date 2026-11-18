#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

no *inserirordenado(no *ini, const char *nome, int pontos){
    no *novo = malloc(sizeof(no));
    if (novo == NULL) return ini;
    strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    novo->pontos = pontos;
    novo->prox = NULL;

    if (ini == NULL || pontos > ini->pontos){
        novo->prox = ini;
        return novo;
    }
    no *aux = ini;
    while (aux->prox != NULL && aux->prox->pontos >= pontos) aux = aux->prox;
    novo->prox = aux->prox;
    aux->prox = novo;
    return ini;
}

no *carregarranking(const char *arquivo){
    no *ini = NULL;
    FILE *f = fopen(arquivo, "r");
    if (f == NULL) return NULL;
    char nome[TAM_NOME];
    int pontos;
    while (fscanf(f, "%19s %d", nome, &pontos) == 2) ini = inserirordenado(ini, nome, pontos);
    fclose(f);
    return ini;
}

void salvarranking(const no *ini, const char *arquivo){
    FILE *f = fopen(arquivo, "w");
    if (f == NULL) return;
    for (const no *p = ini; p != NULL; p = p->prox)
        fprintf(f, "%s %d\n", p->nome, p->pontos);
    fclose(f);
}

void liberarranking(no *ini){
    while (ini != NULL){
        no *prox = ini->prox;
        free(ini);
        ini = prox;
    }
}
