#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

no *inserirordenado(no *ini, char *nome, int pontos){
    no *novo = malloc(sizeof(no));
    if(novo == NULL) return ini;
    strncpy(novo->nome, nome, 19);
    novo->nome[19] = '\0';
    novo->pontos = pontos;
    novo->prox = NULL;
    if(ini == NULL || pontos > ini->pontos){          // era: tacadas < ini->tacadas
        novo->prox = ini;
        return novo;
    }
    no *aux = ini;
    while(aux->prox != NULL && aux->prox->pontos >= pontos) aux = aux->prox;   // era: <= tacadas
    novo->prox = aux->prox;
    aux->prox = novo;
    return ini;
}

no *carregarranking(char *arquivo){
    no *ini = NULL;
    FILE *f = fopen(arquivo, "r");
    if(f == NULL) return NULL;
    char nome[20];
    int t;
    while(fscanf(f, "%19s %d", nome, &t) == 2) ini = inserirordenado(ini, nome, t);
    fclose(f);
    return ini;
}

void salvarranking(no *ini, char *arquivo){
    FILE *f = fopen(arquivo, "w");
    if(f == NULL) return;
    for(no *p = ini; p != NULL; p = p->prox)
        fprintf(f, "%s %d\n", p->nome, p->pontos);
    fclose(f);
}

void liberarranking(no *ini){
    while(ini != NULL){
        no *prox = ini->prox;
        free(ini);
        ini = prox;
    }
}