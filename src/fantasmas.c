#include "fantasmas.h"
#include "grafico_jogo.h"
#include <stdlib.h>


int posicao_livre(float x, float y)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int coluna = (int)((x - x_mapa) / tamanho_bloco);
    int linha = (int)((y - y_mapa) / tamanho_bloco);

    if(linha < 0 || linha >= tamanho_grid ||
       coluna < 0 || coluna >= tamanho_grid)
    {
        return 0;
    }

    if(mapa_info.mapa[linha][coluna].tipo == bloco_parede)
    {
        return 0;
    }

    return 1;
}


int pode_mover_fantasma(struct entidade *fantasma, enum direcao direcao)
{
    float proximo_x = fantasma->pos.x;
    float proximo_y = fantasma->pos.y;

    float metade_tamanho = (tamanho_bloco / 1.5f) / 2.0f;

    switch(direcao)
    {
        case cima:
            proximo_y -= fantasma->velocidade;
            break;

        case baixo:
            proximo_y += fantasma->velocidade;
            break;

        case esquerda:
            proximo_x -= fantasma->velocidade;
            break;

        case direita:
            proximo_x += fantasma->velocidade;
            break;
    }

    if(!posicao_livre(
        proximo_x - metade_tamanho,
        proximo_y - metade_tamanho))
    {
        return 0;
    }

    if(!posicao_livre(
        proximo_x + metade_tamanho,
        proximo_y - metade_tamanho))
    {
        return 0;
    }

    if(!posicao_livre(
        proximo_x - metade_tamanho,
        proximo_y + metade_tamanho))
    {
        return 0;
    }

    if(!posicao_livre(
        proximo_x + metade_tamanho,
        proximo_y + metade_tamanho))
    {
        return 0;
    }

    return 1;
}


enum direcao direcao_oposta(enum direcao direcao)
{
    switch(direcao)
    {
        case cima:
            return baixo;

        case baixo:
            return cima;

        case esquerda:
            return direita;

        case direita:
            return esquerda;
    }

    return direcao;
}


int esta_no_centro_bloco(struct entidade *fantasma)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int x_relativo = (int)(fantasma->pos.x - x_mapa);
    int y_relativo = (int)(fantasma->pos.y - y_mapa);

    int centro_x = x_relativo % tamanho_bloco;
    int centro_y = y_relativo % tamanho_bloco;

    int centro_bloco = tamanho_bloco / 2;

    // Pequena margem porque a velocidade pode fazer
    // o fantasma passar alguns pixels do centro
    int margem = 2;

    if(centro_x >= centro_bloco - margem &&
       centro_x <= centro_bloco + margem &&
       centro_y >= centro_bloco - margem &&
       centro_y <= centro_bloco + margem)
    {
        return 1;
    }

    return 0;
}


int contar_direcoes_livres(struct entidade *fantasma)
{
    int quantidade = 0;

    enum direcao oposta =
        direcao_oposta(fantasma->direcao_atual);

    enum direcao direcoes[4] =
    {
        cima,
        baixo,
        esquerda,
        direita
    };

    for(int i = 0; i < 4; i++)
    {
        if(direcoes[i] == oposta)
        {
            continue;
        }

        if(pode_mover_fantasma(fantasma, direcoes[i]))
        {
            quantidade++;
        }
    }

    return quantidade;
}


void escolher_nova_direcao(struct entidade *fantasma)
{
    enum direcao direcoes_livres[4];
    int quantidade = 0;

    enum direcao oposta =
        direcao_oposta(fantasma->direcao_atual);

    enum direcao direcoes[4] =
    {
        cima,
        baixo,
        esquerda,
        direita
    };

    // Guarda todas as direcoes possiveis,
    // menos a direcao de onde o fantasma veio
    for(int i = 0; i < 4; i++)
    {
        if(direcoes[i] == oposta)
        {
            continue;
        }

        if(pode_mover_fantasma(fantasma, direcoes[i]))
        {
            direcoes_livres[quantidade] = direcoes[i];
            quantidade++;
        }
    }

    // Escolhe uma das direcoes disponiveis
    if(quantidade > 0)
    {
        int escolha = rand() % quantidade;

        fantasma->direcao_atual =
            direcoes_livres[escolha];

        return;
    }

    // Beco sem saida: permite voltar
    if(pode_mover_fantasma(fantasma, oposta))
    {
        fantasma->direcao_atual = oposta;
    }
}


void mover_fantasma(struct entidade *fantasma)
{
    // Se bateu em uma parede, precisa escolher outro caminho
    if(!pode_mover_fantasma(
        fantasma,
        fantasma->direcao_atual))
    {
        escolher_nova_direcao(fantasma);
    }

    // Se estiver no centro de um bloco e houver mais de
    // uma opcao, significa que chegou a um cruzamento
    else if(esta_no_centro_bloco(fantasma))
    {
        int quantidade =
            contar_direcoes_livres(fantasma);

        if(quantidade > 1)
        {
            escolher_nova_direcao(fantasma);
        }
    }

    // Confere novamente antes de andar
    if(!pode_mover_fantasma(
        fantasma,
        fantasma->direcao_atual))
    {
        return;
    }

    switch(fantasma->direcao_atual)
    {
        case cima:
            fantasma->pos.y -= fantasma->velocidade;
            break;

        case baixo:
            fantasma->pos.y += fantasma->velocidade;
            break;

        case esquerda:
            fantasma->pos.x -= fantasma->velocidade;
            break;

        case direita:
            fantasma->pos.x += fantasma->velocidade;
            break;
    }
}


void atualizar_fantasmas(void)
{
    for(int i = 0; i < quantidade_entidades; i++)
    {
        if(entidades_para_render[i].tipo != entidade_fantasma)
        {
            continue;
        }

        mover_fantasma(&entidades_para_render[i]);
    }
}
