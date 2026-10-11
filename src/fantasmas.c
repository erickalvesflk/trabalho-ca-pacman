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


// Calcula uma distancia simples entre dois pontos.
// Nao precisamos de raiz quadrada para comparar as distancias.
float calcular_distancia(
    float x1,
    float y1,
    float x2,
    float y2)
{
    float diferenca_x = x2 - x1;
    float diferenca_y = y2 - y1;

    return
        (diferenca_x * diferenca_x) +
        (diferenca_y * diferenca_y);
}


void escolher_nova_direcao(struct entidade *fantasma)
{
    struct entidade *pacman = &entidades_para_render[0];

    enum direcao oposta =
        direcao_oposta(fantasma->direcao_atual);

    enum direcao direcoes[4] =
    {
        cima,
        baixo,
        esquerda,
        direita
    };

    enum direcao melhor_direcao =
        fantasma->direcao_atual;

    float melhor_distancia = -1.0f;

    int encontrou_direcao = 0;


    // Testa cada caminho disponivel
    for(int i = 0; i < 4; i++)
    {
        enum direcao direcao_testada = direcoes[i];

        // Evita voltar imediatamente pelo mesmo caminho
        if(direcao_testada == oposta)
        {
            continue;
        }

        if(!pode_mover_fantasma(
            fantasma,
            direcao_testada))
        {
            continue;
        }


        float proximo_x = fantasma->pos.x;
        float proximo_y = fantasma->pos.y;

        // Simula a proxima posicao do fantasma
        switch(direcao_testada)
        {
            case cima:
                proximo_y -= tamanho_bloco;
                break;

            case baixo:
                proximo_y += tamanho_bloco;
                break;

            case esquerda:
                proximo_x -= tamanho_bloco;
                break;

            case direita:
                proximo_x += tamanho_bloco;
                break;
        }


        float distancia =
            calcular_distancia(
                proximo_x,
                proximo_y,
                pacman->pos.x,
                pacman->pos.y
            );


        // Guarda o caminho que mais aproxima do Pac-Man
        if(!encontrou_direcao ||
           distancia < melhor_distancia)
        {
            melhor_distancia = distancia;
            melhor_direcao = direcao_testada;
            encontrou_direcao = 1;
        }
    }


    if(encontrou_direcao)
    {
        fantasma->direcao_atual = melhor_direcao;
        return;
    }


    // Se chegou em um beco sem saida,
    // permite voltar pelo caminho anterior
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
