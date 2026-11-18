#ifndef RANKING_H
#define RANKING_H

#define TAM_NOME 20

typedef struct no{
    char nome[TAM_NOME];
    int pontos;
    struct no *prox;
}no;

no *inserirordenado(no *ini, const char *nome, int pontos);
no *carregarranking(const char *arquivo);
void salvarranking(const no *ini, const char *arquivo);
void liberarranking(no *ini);

#endif
