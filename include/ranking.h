#ifndef RANKING_H
#define RANKING_H
typedef struct no{
    char nome[20];
    int tacadas;
    struct no *prox;
}no;

no *inserirordenado(no *ini, char *nome, int tacadas);
no *carregarranking(char *arquivo);
void salvarranking(no *ini, char *arquivo);
void liberarranking(no *ini);




#endif