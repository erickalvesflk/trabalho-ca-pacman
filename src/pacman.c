#include "pacman.h"
#include "grafico_jogo.h"


// Direcao que o jogador quer seguir assim que for possivel
enum direcao direcao_desejada = direita;


// Pac-Man comeca a partida com tres vidas
int vidas_pacman = 3;


// Tempo restante de protecao depois de uma colisao
float tempo_protecao_pacman = 0.0f;


int linha_tem_tunel(int linha)
{
    if(linha < 0 || linha >= tamanho_grid)
    {
        return 0;
    }

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


int pode_mover_pacman(
    struct entidade *pacman,
    enum direcao direcao)
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

    if(pacman->pos.x < limite_esquerdo)
    {
        pacman->pos.x = limite_direito - 1;
    }
    else if(pacman->pos.x > limite_direito)
    {
        pacman->pos.x = limite_esquerdo + 1;
    }
}


void mover_pacman(struct entidade *pacman)
{
    if(pode_mover_pacman(
        pacman,
        direcao_desejada))
    {
        pacman->direcao_atual =
            direcao_desejada;
    }

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

    verificar_tunel_pacman(pacman);
}


// Verifica se Pac-Man esta encostando
// em algum dos fantasmas
int pacman_colidiu_com_fantasma(void)
{
    struct entidade *pacman =
        &entidades_para_render[0];

    float distancia_colisao =
        tamanho_bloco / 1.5f;


    for(int i = 1; i < quantidade_entidades; i++)
    {
        struct entidade *fantasma =
            &entidades_para_render[i];

        if(fantasma->tipo != entidade_fantasma)
        {
            continue;
        }


        float diferenca_x =
            pacman->pos.x - fantasma->pos.x;

        float diferenca_y =
            pacman->pos.y - fantasma->pos.y;


        if(diferenca_x < 0)
        {
            diferenca_x = -diferenca_x;
        }

        if(diferenca_y < 0)
        {
            diferenca_y = -diferenca_y;
        }


        if(diferenca_x <= distancia_colisao &&
           diferenca_y <= distancia_colisao)
        {
            return 1;
        }
    }


    return 0;
}


void atualizar_colisao_pacman(void)
{
    // Reduz o tempo de protecao
    if(tempo_protecao_pacman > 0.0f)
    {
        tempo_protecao_pacman -= GetFrameTime();

        if(tempo_protecao_pacman < 0.0f)
        {
            tempo_protecao_pacman = 0.0f;
        }

        return;
    }


    if(pacman_colidiu_com_fantasma())
    {
        vidas_pacman--;

        // Um segundo sem poder perder outra vida
        tempo_protecao_pacman = 1.0f;


        // Protecao para nao ficar com vidas negativas
        if(vidas_pacman < 0)
        {
            vidas_pacman = 0;
        }
    }
}


void atualizar_pacman(void)
{
    struct entidade *pacman =
        &entidades_para_render[0];

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
