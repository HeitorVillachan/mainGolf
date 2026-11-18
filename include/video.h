#ifndef VIDEO_H
#define VIDEO_H
#include "raylib.h"

#define LARGURA_MENU 800
#define ALTURA_MENU  480

typedef struct video{
    int largura, altura;    
    float escala;          
    Vector2 offset;         
}video;

void iniciavideo(video *v, int largura, int altura, const char *titulo);
void definirresolucao(video *v, int largura, int altura);
void atualizavideo(video *v);    
void comecardesenho(video *v);      
void terminardesenho(void);        
bool telacheia(void);

#endif
