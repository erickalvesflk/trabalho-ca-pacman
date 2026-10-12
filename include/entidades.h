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

struct entidade
{
    enum tipo_entidade tipo;
    Vector2 pos;
    Color cor;
    enum direcao direcao_atual;
    float velocidade;
};

extern struct entidade entidades_para_render[quantidade_entidades];

void gerar_entidades(void);

#endif
