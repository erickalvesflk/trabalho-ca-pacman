#include "fantasmas.h"
#include "grafico_jogo.h"
#include "stdio.h"

// Representa uma posicao dentro do grid
struct posicao_grid
{
    int linha;
    int coluna;
};


// Personalidades dos fantasmas
enum personalidade_fantasma
{
    perseguidor,
    miope,
    previsor,
    emboscador,
    imprevisivel
};


// Destino atual usado pelo fantasma roxo
int roxo_destino_linha = -1;
int roxo_destino_coluna = -1;

// Quantas decisoes o roxo mantem o mesmo destino
int roxo_decisoes_restantes = 0;


int linha_tem_tunel_fantasma(int linha)
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

int coluna_tem_tunel_fantasma(int coluna)
{
    if(coluna < 0 || coluna >= tamanho_grid)
    {
        return 0;
    }

    if(mapa_info.mapa[0][coluna].tipo == bloco_parede ||
       mapa_info.mapa[tamanho_grid - 1][coluna].tipo == bloco_parede)
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
        if(coluna_tem_tunel_fantasma(coluna))
        {
            return 1;
        }
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
// para o outro lado quando existe tunel
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
// Converte uma linha que saiu pela lateral
// para o outro lado quando existe tunel
int ajustar_linha_tunel(int linha, int coluna)
{
    if(!coluna_tem_tunel_fantasma(coluna))
    {
        return linha;
    }

    if(linha < 0)
    {
        return tamanho_grid - 1;
    }

    if(linha >= tamanho_grid)
    {
        return 0;
    }

    return linha;
}


// Usa BFS para descobrir o primeiro passo
// do menor caminho ate uma celula de destino
enum direcao buscar_direcao_bfs(
    struct entidade *fantasma,
    int destino_linha,
    int destino_coluna)
{
    int inicio_linha;
    int inicio_coluna;

    posicao_para_grid(
        fantasma->pos,
        &inicio_linha,
        &inicio_coluna
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
            int nova_linha = atual.linha + movimento_linha[i];
            int nova_coluna = atual.coluna + movimento_coluna[i];

            nova_coluna = ajustar_coluna_tunel(
                    nova_linha,
                    nova_coluna
            );
            nova_linha = ajustar_linha_tunel(
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


    // Caso especial: primeiro passo usa o tunel
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

    if(inicio_linha == 0 &&
       passo.linha == tamanho_grid - 1)
    {
        return cima;
    }

    if(inicio_linha == tamanho_grid - 1 &&
       passo.linha == 0)
    {
        return baixo;
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


enum personalidade_fantasma obter_personalidade(
    int indice)
{
    switch(indice)
    {
        case 1:
            return perseguidor;

        case 2:
            return miope;

        case 3:
            return previsor;

        case 4:
            return emboscador;

        case 5:
            return imprevisivel;

        default:
            return perseguidor;
    }
}


// Procura uma celula valida na direcao atual
// do Pac-Man
void calcular_destino_a_frente(
    int pacman_linha,
    int pacman_coluna,
    enum direcao direcao,
    int distancia,
    int *destino_linha,
    int *destino_coluna)
{
    *destino_linha = pacman_linha;
    *destino_coluna = pacman_coluna;


    for(int i = 0; i < distancia; i++)
    {
        int nova_linha = *destino_linha;
        int nova_coluna = *destino_coluna;


        switch(direcao)
        {
            case cima:
                nova_linha--;
                break;

            case baixo:
                nova_linha++;
                break;

            case esquerda:
                nova_coluna--;
                break;

            case direita:
                nova_coluna++;
                break;
        }


        nova_coluna =
            ajustar_coluna_tunel(
                nova_linha,
                nova_coluna
            );
        nova_linha =
            ajustar_linha_tunel(
                nova_linha,
                nova_coluna
            );


        // Para na ultima celula valida
        // antes de uma parede
        if(!celula_livre(
            nova_linha,
            nova_coluna))
        {
            break;
        }


        *destino_linha = nova_linha;
        *destino_coluna = nova_coluna;
    }
}


// Escolhe uma direcao diferente da ideal
// para representar a miopia do amarelo
enum direcao erro_fantasma_amarelo(
    struct entidade *fantasma,
    enum direcao direcao_ideal)
{
    enum direcao direcoes[4] =
    {
        cima,
        baixo,
        esquerda,
        direita
    };

    enum direcao opcoes[4];
    int quantidade_opcoes = 0;


    for(int i = 0; i < 4; i++)
    {
        enum direcao direcao_teste =
            direcoes[i];


        if(direcao_teste == direcao_ideal)
        {
            continue;
        }


        if(pode_mover_fantasma(
            fantasma,
            direcao_teste))
        {
            opcoes[quantidade_opcoes] =
                direcao_teste;

            quantidade_opcoes++;
        }
    }


    if(quantidade_opcoes == 0)
    {
        return direcao_ideal;
    }


    int escolha =
        GetRandomValue(
            0,
            quantidade_opcoes - 1
        );


    return opcoes[escolha];
}


// Escolhe um novo objetivo para o fantasma roxo
void escolher_destino_roxo(
    int pacman_linha,
    int pacman_coluna)
{
    // Metade das vezes ele resolve perseguir
    // diretamente o Pac-Man
    int perseguir =
        GetRandomValue(0, 1);


    if(perseguir)
    {
        roxo_destino_linha = pacman_linha;
        roxo_destino_coluna = pacman_coluna;
    }
    else
    {
        // Tenta encontrar uma celula livre aleatoria
        for(int tentativa = 0; tentativa < 50; tentativa++)
        {
            int linha =
                GetRandomValue(
                    0,
                    tamanho_grid - 1
                );

            int coluna =
                GetRandomValue(
                    0,
                    tamanho_grid - 1
                );


            if(celula_livre(linha, coluna))
            {
                roxo_destino_linha = linha;
                roxo_destino_coluna = coluna;
                break;
            }
        }


        // Protecao caso nenhuma celula seja encontrada
        if(roxo_destino_linha == -1 ||
           roxo_destino_coluna == -1)
        {
            roxo_destino_linha = pacman_linha;
            roxo_destino_coluna = pacman_coluna;
        }
    }


    // Mantem a escolha por algumas decisoes
    // para nao trocar de ideia o tempo inteiro
    roxo_decisoes_restantes =
        GetRandomValue(3, 7);
}


// Decide qual direcao cada personalidade deseja seguir
enum direcao decidir_direcao_fantasma(
    struct entidade *fantasma,
    int indice)
{
    struct entidade *pacman =
        &entidades_para_render[0];

    int pacman_linha;
    int pacman_coluna;

    posicao_para_grid(
        pacman->pos,
        &pacman_linha,
        &pacman_coluna
    );


    enum personalidade_fantasma personalidade =
        obter_personalidade(indice);


    switch(personalidade)
    {
        // Vermelho
        // Vai diretamente para o Pac-Man
        case perseguidor:
        {
            return buscar_direcao_bfs(
                fantasma,
                pacman_linha,
                pacman_coluna
            );
        }


        // Amarelo
        // Pode errar quando esta perto
        case miope:
        {
            enum direcao direcao_ideal =
                buscar_direcao_bfs(
                    fantasma,
                    pacman_linha,
                    pacman_coluna
                );


            int fantasma_linha;
            int fantasma_coluna;

            posicao_para_grid(
                fantasma->pos,
                &fantasma_linha,
                &fantasma_coluna
            );


            int distancia_linha =
                fantasma_linha - pacman_linha;

            int distancia_coluna =
                fantasma_coluna - pacman_coluna;


            if(distancia_linha < 0)
            {
                distancia_linha =
                    -distancia_linha;
            }

            if(distancia_coluna < 0)
            {
                distancia_coluna =
                    -distancia_coluna;
            }


            int distancia =
                distancia_linha +
                distancia_coluna;


            if(distancia <= 6)
            {
                int chance =
                    GetRandomValue(1, 100);

                if(chance <= 15)
                {
                    return erro_fantasma_amarelo(
                        fantasma,
                        direcao_ideal
                    );
                }
            }


            return direcao_ideal;
        }


        // Verde
        // Tenta prever onde o Pac-Man estara
        case previsor:
        {
            int destino_linha;
            int destino_coluna;


            calcular_destino_a_frente(
                pacman_linha,
                pacman_coluna,
                pacman->direcao_atual,
                4,
                &destino_linha,
                &destino_coluna
            );


            return buscar_direcao_bfs(
                fantasma,
                destino_linha,
                destino_coluna
            );
        }


        // Azul
        // Tenta chegar bem mais a frente
        // para interceptar o Pac-Man
        case emboscador:
        {
            int destino_linha;
            int destino_coluna;


            calcular_destino_a_frente(
                pacman_linha,
                pacman_coluna,
                pacman->direcao_atual,
                7,
                &destino_linha,
                &destino_coluna
            );


            return buscar_direcao_bfs(
                fantasma,
                destino_linha,
                destino_coluna
            );
        }


        // Roxo
        // Alterna entre perseguicao e
        // objetivos aleatorios pelo mapa
        case imprevisivel:
        {
            if(roxo_decisoes_restantes <= 0 ||
               !celula_livre(
                   roxo_destino_linha,
                   roxo_destino_coluna))
            {
                escolher_destino_roxo(
                    pacman_linha,
                    pacman_coluna
                );
            }


            roxo_decisoes_restantes--;


            return buscar_direcao_bfs(
                fantasma,
                roxo_destino_linha,
                roxo_destino_coluna
            );
        }
    }


    return fantasma->direcao_atual;
}


void verificar_tunel_fantasma(struct entidade *fantasma)
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    int linha = (int)((fantasma->pos.y - y_mapa) / tamanho_bloco);
    int coluna = (int)((fantasma->pos.x - x_mapa) / tamanho_bloco);

    if(linha_tem_tunel_fantasma(linha))
    {
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

    if(coluna_tem_tunel_fantasma(coluna))
    {
        float limite_topo = y_mapa;
        float limite_base = y_mapa + tamanho_mapa;
        
        if(fantasma->pos.y < limite_topo)
        {
            fantasma->pos.y = limite_base - 1;
        }
        else if(fantasma->pos.y > limite_base)
        {
            fantasma->pos.y = limite_topo + 1;
        }
    }

}


void mover_fantasma(
    struct entidade *fantasma,
    int indice)
{
    
    if(esta_no_centro_bloco(fantasma))
    {
        enum direcao nova_direcao =
            decidir_direcao_fantasma(
                fantasma,
                indice
            );

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
            decidir_direcao_fantasma(
                fantasma,
                indice
            );

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
            &entidades_para_render[i],
            i
        );
    }
}
