#ifndef PONTOS_H
#define PONTOS_H

typedef struct placar{
    int total;
    int sequencia;       
    int multiplicador;  
    int ultimafase;      
}placar;

void iniciaplacar(placar *p);
int pontuarfase(placar *p, int tacadas, int par, int quedas);

#endif