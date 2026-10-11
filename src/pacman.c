#include "pacman.h"
#include "raylib.h"


void mover_pacman(struct entidade *pacman)
{
    switch(pacman->direcao_atual)
    {
        case cima:
            pacman->pos.y -= pacman->velocidade;
            break;

        case baixo:
            pacman->pos.y += pacman->velocidade;
            break;

        case esquerda:
            pacman->pos.x -= pacman->velocidade;
            break;

        case direita:
            pacman->pos.x += pacman->velocidade;
            break;
    }
}


void atualizar_pacman(void)
{
    struct entidade *pacman = &entidades_para_render[0];

    if(pacman->tipo != entidade_pacman)
    {
        return;
    }

    // Escolhe a direcao pelas setas do teclado
    if(IsKeyDown(KEY_UP))
    {
        pacman->direcao_atual = cima;
    }
    else if(IsKeyDown(KEY_DOWN))
    {
        pacman->direcao_atual = baixo;
    }
    else if(IsKeyDown(KEY_LEFT))
    {
        pacman->direcao_atual = esquerda;
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        pacman->direcao_atual = direita;
    }

    mover_pacman(pacman);
}
