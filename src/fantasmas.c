#include "fantasmas.h"
#include "grafico_jogo.h"


int posicao_livre(float x, float y)
{
    // Inicio do mapa na tela
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    // Converte a posicao em pixels para linha e coluna do mapa
    int coluna = (int)((x - x_mapa) / tamanho_bloco);
    int linha = (int)((y - y_mapa) / tamanho_bloco);

    // Impede que o fantasma saia dos limites do mapa
    if(linha < 0 || linha >= tamanho_grid ||
       coluna < 0 || coluna >= tamanho_grid)
    {
        return 0;
    }

    // Impede que o fantasma entre em uma parede
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

    // Metade do tamanho usado para desenhar o fantasma
    float metade_tamanho = (tamanho_bloco / 1.5f) / 2.0f;

    // Calcula a proxima posicao do fantasma
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

    // Verifica os quatro cantos do fantasma
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


void escolher_nova_direcao(struct entidade *fantasma)
{
    enum direcao direcoes[4] =
    {
        cima,
        baixo,
        esquerda,
        direita
    };

    // Procura uma direcao livre
    for(int i = 0; i < 4; i++)
    {
        if(pode_mover_fantasma(fantasma, direcoes[i]))
        {
            fantasma->direcao_atual = direcoes[i];
            return;
        }
    }
}


void mover_fantasma(struct entidade *fantasma)
{
    // Se encontrou uma parede, procura outra direcao
    if(!pode_mover_fantasma(fantasma, fantasma->direcao_atual))
    {
        escolher_nova_direcao(fantasma);
    }

    // Verifica novamente antes de movimentar
    if(!pode_mover_fantasma(fantasma, fantasma->direcao_atual))
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
