#ifndef MENU_H
#define MENU_H
#include "raylib.h"
#include "ranking.h"

enum { OPC_NENHUMA = -1, OPC_JOGAR, OPC_RANKING, OPC_SAIR };

typedef struct menu{
    Font fonte;
    bool fonteok;
    int opcao;
}menu;

void iniciamenu(menu *mn);
void liberamenu(menu *mn);
int atualizamenu(menu *mn);         
void desenhamenu(menu *mn);
bool atualizaranking(void);     
void desenharranking(menu *mn, no *rank);

#endif