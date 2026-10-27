#include "mapa.h"
#include <stdlib.h>

int **criamatriz(int linhas, int colunas){
    
    int **m = malloc(linhas * sizeof(int*));

    if (m == NULL) return NULL;
    
    for(int i = 0; i<linhas;i++){
        m[i] = malloc(colunas * sizeof(int));

        if (m[i] == NULL) {
            for (int k = 0; k < i; k++) free(m[k]);
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