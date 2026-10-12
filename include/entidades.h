#ifndef ENTIDADES_H
#define ENTIDADES_H

#include "raylib.h"

#define quantidade_entidades 6

enum direcao
{
    direita,
    baixo,
    esquerda,
    cima
};

enum tipo_entidade
{
    entidade_fantasma,
    entidade_pacman,
    entidade_vazia
};

struct quadros_entidade
{
    Texture2D textura;
    int quant_quadros;
};

struct entidade
{
    enum tipo_entidade tipo;
    
    struct quadros_entidade sprites;
    Color cor;

    enum direcao direcao_atual;
    float velocidade;
    Vector2 pos;
};

extern struct entidade entidades_para_render[quantidade_entidades];

void gerar_entidades(void);

#endif
