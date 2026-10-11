#include "pacman.h"
#include "grafico_jogo.h"


// Direcao que o jogador quer seguir assim que for possivel
enum direcao direcao_desejada = direita;


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
    // Primeiro tenta usar a direcao escolhida pelo jogador
    if(pode_mover_pacman(pacman, direcao_desejada))
    {
        pacman->direcao_atual = direcao_desejada;
    }

    // Se a direcao atual estiver bloqueada, para
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

    // Guarda a direcao que o jogador deseja seguir
    if(IsKeyDown(KEY_UP))
    {
        direcao_desejada = cima;
    }
    else if(IsKeyDown(KEY_DOWN))
    {
        direcao_desejada = baixo;
    }
    else if(IsKeyDown(KEY_LEFT))
    {
        direcao_desejada = esquerda;
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        direcao_desejada = direita;
    }

    mover_pacman(pacman);
}
