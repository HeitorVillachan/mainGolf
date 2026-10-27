#ifndef MAPA_H
#define MAPA_H
int **criamatriz(int linhas, int colunas);

typedef enum{ //nesse enum ele enumera sozinho. ai no caso grama é 0 areia é 1 e etc.
    grama,     
    areia,     
    gelo,      
    barro,     
    parede,    
    agua,      
    buraco
    
}pisos;

typedef struct mapa{
    int linha, coluna, **chao;

}mapa;

void liberamatriz(int **m, int linhas);
mapa* carregarMapa(const char *caminhoArquivo);
void liberarMapa(mapa *m);

#endif