#include "pacman.h"
#include "grafico_jogo.h"


// Direcao que o jogador quer seguir assim que for possivel
enum direcao direcao_desejada = direita;


int linha_tem_tunel(int linha)
{
    if(linha < 0 || linha >= tamanho_grid)
    {
        return 0;
    }

    // So existe tunel se as duas extremidades
    // da linha nao forem paredes
    if(mapa_info.mapa[linha][0].tipo == bloco_parede ||
       mapa_info.mapa[linha][tamanho_grid - 1].tipo == bloco_parede)
    {
        return 0;
    }

    return 1;
}


int posicao_livre_pacman(float x, float y)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int coluna = (int)((x - x_mapa) / tamanho_bloco);
    int linha = (int)((y - y_mapa) / tamanho_bloco);

    if(linha < 0 || linha >= tamanho_grid)
    {
        return 0;
    }

    // Permite ultrapassar horizontalmente apenas
    // quando estiver em uma linha de tunel
    if(coluna < 0 || coluna >= tamanho_grid)
    {
        if(linha_tem_tunel(linha))
        {
            return 1;
        }

        return 0;
    }

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


void verificar_tunel_pacman(struct entidade *pacman)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int linha =
        (int)((pacman->pos.y - y_mapa) / tamanho_bloco);

    if(!linha_tem_tunel(linha))
    {
        return;
    }

    float limite_esquerdo = x_mapa;
    float limite_direito = x_mapa + tamanho_mapa;


    // Saiu completamente pela esquerda
    if(pacman->pos.x < limite_esquerdo)
    {
        pacman->pos.x = limite_direito - 1;
    }

    // Saiu completamente pela direita
    else if(pacman->pos.x > limite_direito)
    {
        pacman->pos.x = limite_esquerdo + 1;
    }
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


    // Depois do movimento verifica se atravessou um tunel
    verificar_tunel_pacman(pacman);
}


void atualizar_pacman(void)
{
    struct entidade *pacman = &entidades_para_render[0];

    if(pacman->tipo != entidade_pacman)
    {
        return;
    }

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
