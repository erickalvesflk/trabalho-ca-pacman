#include "fantasmas.h"
#include "grafico_jogo.h"


// Representa uma posicao dentro do grid
struct posicao_grid
{
    int linha;
    int coluna;
};


int linha_tem_tunel_fantasma(int linha)
{
    if(linha < 0 || linha >= tamanho_grid)
    {
        return 0;
    }

    // O tunel existe somente quando as duas
    // extremidades da linha sao livres
    if(mapa_info.mapa[linha][0].tipo == bloco_parede ||
       mapa_info.mapa[linha][tamanho_grid - 1].tipo == bloco_parede)
    {
        return 0;
    }

    return 1;
}


int posicao_livre(float x, float y)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int coluna = (int)((x - x_mapa) / tamanho_bloco);
    int linha = (int)((y - y_mapa) / tamanho_bloco);

    if(linha < 0 || linha >= tamanho_grid)
    {
        return 0;
    }

    // Permite sair horizontalmente apenas
    // quando estiver em uma linha de tunel
    if(coluna < 0 || coluna >= tamanho_grid)
    {
        if(linha_tem_tunel_fantasma(linha))
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


void posicao_para_grid(
    Vector2 posicao,
    int *linha,
    int *coluna)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    *coluna =
        (int)((posicao.x - x_mapa) / tamanho_bloco);

    *linha =
        (int)((posicao.y - y_mapa) / tamanho_bloco);
}


int pode_mover_fantasma(
    struct entidade *fantasma,
    enum direcao direcao)
{
    float proximo_x = fantasma->pos.x;
    float proximo_y = fantasma->pos.y;

    float metade_tamanho =
        (tamanho_bloco / 1.5f) / 2.0f;

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


int esta_no_centro_bloco(struct entidade *fantasma)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int x_relativo =
        (int)(fantasma->pos.x - x_mapa);

    int y_relativo =
        (int)(fantasma->pos.y - y_mapa);

    int centro_x = x_relativo % tamanho_bloco;
    int centro_y = y_relativo % tamanho_bloco;

    int centro_bloco = tamanho_bloco / 2;
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


int celula_livre(int linha, int coluna)
{
    if(linha < 0 || linha >= tamanho_grid ||
       coluna < 0 || coluna >= tamanho_grid)
    {
        return 0;
    }

    if(mapa_info.mapa[linha][coluna].tipo ==
       bloco_parede)
    {
        return 0;
    }

    return 1;
}


// Converte uma coluna que saiu pela lateral
// para o outro lado do mapa quando existe tunel
int ajustar_coluna_tunel(int linha, int coluna)
{
    if(!linha_tem_tunel_fantasma(linha))
    {
        return coluna;
    }

    if(coluna < 0)
    {
        return tamanho_grid - 1;
    }

    if(coluna >= tamanho_grid)
    {
        return 0;
    }

    return coluna;
}


// Usa BFS para descobrir o primeiro passo
// do menor caminho ate o Pac-Man
enum direcao buscar_direcao_bfs(
    struct entidade *fantasma)
{
    struct entidade *pacman =
        &entidades_para_render[0];

    int inicio_linha;
    int inicio_coluna;

    int destino_linha;
    int destino_coluna;

    posicao_para_grid(
        fantasma->pos,
        &inicio_linha,
        &inicio_coluna
    );

    posicao_para_grid(
        pacman->pos,
        &destino_linha,
        &destino_coluna
    );


    if(!celula_livre(inicio_linha, inicio_coluna) ||
       !celula_livre(destino_linha, destino_coluna))
    {
        return fantasma->direcao_atual;
    }


    int visitado[tamanho_grid][tamanho_grid] = {0};

    struct posicao_grid anterior
        [tamanho_grid][tamanho_grid];

    struct posicao_grid fila
        [tamanho_grid * tamanho_grid];

    int inicio_fila = 0;
    int fim_fila = 0;


    fila[fim_fila].linha = inicio_linha;
    fila[fim_fila].coluna = inicio_coluna;
    fim_fila++;

    visitado[inicio_linha][inicio_coluna] = 1;

    anterior[inicio_linha][inicio_coluna].linha = -1;
    anterior[inicio_linha][inicio_coluna].coluna = -1;


    int movimento_linha[4] =
    {
        -1,
        1,
        0,
        0
    };

    int movimento_coluna[4] =
    {
        0,
        0,
        -1,
        1
    };


    while(inicio_fila < fim_fila)
    {
        struct posicao_grid atual =
            fila[inicio_fila];

        inicio_fila++;


        if(atual.linha == destino_linha &&
           atual.coluna == destino_coluna)
        {
            break;
        }


        for(int i = 0; i < 4; i++)
        {
            int nova_linha =
                atual.linha + movimento_linha[i];

            int nova_coluna =
                atual.coluna + movimento_coluna[i];


            // Se tentou sair pela lateral em uma
            // linha de tunel, aparece do outro lado
            nova_coluna =
                ajustar_coluna_tunel(
                    nova_linha,
                    nova_coluna
                );


            if(!celula_livre(
                nova_linha,
                nova_coluna))
            {
                continue;
            }


            if(visitado[nova_linha][nova_coluna])
            {
                continue;
            }


            visitado[nova_linha][nova_coluna] = 1;

            anterior[nova_linha][nova_coluna] =
                atual;

            fila[fim_fila].linha = nova_linha;
            fila[fim_fila].coluna = nova_coluna;

            fim_fila++;
        }
    }


    if(!visitado[destino_linha][destino_coluna])
    {
        return fantasma->direcao_atual;
    }


    struct posicao_grid passo;

    passo.linha = destino_linha;
    passo.coluna = destino_coluna;


    while(1)
    {
        struct posicao_grid pai =
            anterior[passo.linha][passo.coluna];


        if(pai.linha == inicio_linha &&
           pai.coluna == inicio_coluna)
        {
            break;
        }


        if(pai.linha == -1 ||
           pai.coluna == -1)
        {
            return fantasma->direcao_atual;
        }


        passo = pai;
    }


    // Caso especial: o primeiro passo usa o tunel
    if(inicio_coluna == 0 &&
       passo.coluna == tamanho_grid - 1)
    {
        return esquerda;
    }

    if(inicio_coluna == tamanho_grid - 1 &&
       passo.coluna == 0)
    {
        return direita;
    }


    if(passo.linha < inicio_linha)
    {
        return cima;
    }

    if(passo.linha > inicio_linha)
    {
        return baixo;
    }

    if(passo.coluna < inicio_coluna)
    {
        return esquerda;
    }

    if(passo.coluna > inicio_coluna)
    {
        return direita;
    }


    return fantasma->direcao_atual;
}


void verificar_tunel_fantasma(struct entidade *fantasma)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int linha =
        (int)((fantasma->pos.y - y_mapa) / tamanho_bloco);

    if(!linha_tem_tunel_fantasma(linha))
    {
        return;
    }


    float limite_esquerdo = x_mapa;
    float limite_direito = x_mapa + tamanho_mapa;


    if(fantasma->pos.x < limite_esquerdo)
    {
        fantasma->pos.x = limite_direito - 1;
    }

    else if(fantasma->pos.x > limite_direito)
    {
        fantasma->pos.x = limite_esquerdo + 1;
    }
}


void mover_fantasma(struct entidade *fantasma)
{
    if(esta_no_centro_bloco(fantasma))
    {
        enum direcao nova_direcao =
            buscar_direcao_bfs(fantasma);

        if(pode_mover_fantasma(
            fantasma,
            nova_direcao))
        {
            fantasma->direcao_atual =
                nova_direcao;
        }
    }


    if(!pode_mover_fantasma(
        fantasma,
        fantasma->direcao_atual))
    {
        enum direcao nova_direcao =
            buscar_direcao_bfs(fantasma);

        if(pode_mover_fantasma(
            fantasma,
            nova_direcao))
        {
            fantasma->direcao_atual =
                nova_direcao;
        }
        else
        {
            return;
        }
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


    verificar_tunel_fantasma(fantasma);
}


void atualizar_fantasmas(void)
{
    for(int i = 0; i < quantidade_entidades; i++)
    {
        if(entidades_para_render[i].tipo !=
           entidade_fantasma)
        {
            continue;
        }

        mover_fantasma(
            &entidades_para_render[i]
        );
    }
}
