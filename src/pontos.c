#include "pontos.h"
#include <stdbool.h>

#define PONTOS_BASE        100
#define BONUS_POR_TACADA   50
#define PENAL_POR_TACADA   20
#define BONUS_TACADA_UNICA 200
#define BONUS_LIMPA        50
#define MULT_MAX           5

void iniciaplacar(placar *p){
    p->total = 0;
    p->sequencia = 0;
    p->multiplicador = 1;
    p->ultimafase = 0;
}

int pontuarfase(placar *p, int tacadas, int par, int quedas){
    bool nopar = tacadas <= par;
    int pts = PONTOS_BASE;

    if (nopar) pts += (par - tacadas) * BONUS_POR_TACADA;
    else       pts -= (tacadas - par) * PENAL_POR_TACADA;

    if (tacadas == 1) pts += BONUS_TACADA_UNICA;
    if (quedas == 0)  pts += BONUS_LIMPA;
    if (pts < 0) pts = 0;

    p->multiplicador = 1 + p->sequencia;
    if (p->multiplicador > MULT_MAX) p->multiplicador = MULT_MAX;
    pts *= p->multiplicador;

    if (nopar) p->sequencia++;
    else       p->sequencia = 0;

    p->total += pts;
    p->ultimafase = pts;
    return pts;
}