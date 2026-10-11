#include "pacman.h"
#include "grafico_jogo.h"


int posicao_livre_pacman(float x, float y)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int coluna = (int)((x - x_mapa) / tamanho_bloco);
    int linha = (int)((y - y_mapa) / tamanho_bloco);

    // Impede que o Pac-Man saia dos limites do mapa
    if(linha < 0 || linha >= tamanho_grid ||
       coluna < 0 || coluna >= tamanho_grid)
    {
        return 0;
    }

    // Impede que o Pac-Man entre em uma parede
    if(mapa_info.mapa[linha][coluna].tipo == bloco_parede)
    {
        return 0;
    }

    return 1;
}


int pode_mover_pacman(struct entidade *pacman, enum direcao direcao)
{
    float proximo_x = pacman->pos.x;
    float proximo_y = pacman->pos.y;

    // Mesmo tamanho usado para desenhar o Pac-Man
    float raio = tamanho_bloco / 2.5f;

    switch(direcao)
    {
        case cima:
            proximo_y -= pacman->velocidade;
            break;

        case baixo:
            proximo_y += pacman->velocidade;
            break;

        case esquerda:
            proximo_x -= pacman->velocidade;
            break;

        case direita:
            proximo_x += pacman->velocidade;
            break;
    }

    // Verifica as extremidades do Pac-Man
    if(!posicao_livre_pacman(
        proximo_x - raio,
        proximo_y - raio))
    {
        return 0;
    }

    if(!posicao_livre_pacman(
        proximo_x + raio,
        proximo_y - raio))
    {
        return 0;
    }

    if(!posicao_livre_pacman(
        proximo_x - raio,
        proximo_y + raio))
    {
        return 0;
    }

    if(!posicao_livre_pacman(
        proximo_x + raio,
        proximo_y + raio))
    {
        return 0;
    }

    return 1;
}


void mover_pacman(struct entidade *pacman)
{
    // So movimenta se o caminho estiver livre
    if(!pode_mover_pacman(
        pacman,
        pacman->direcao_atual))
    {
        return;
    }

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
