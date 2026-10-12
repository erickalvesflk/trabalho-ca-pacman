#include "entidades.h"
#include "geranciador_de_mapa.h"

struct entidade entidades_para_render[quantidade_entidades];

void gerar_entidades()
{
    for(int i = 0; i < quantidade_entidades; ++i)
    {
        entidades_para_render[i].tipo = entidade_vazia;
    }


    // Pac-Man
    struct entidade pacman;

    pacman.tipo = entidade_pacman;
    pacman.pos = (Vector2){0, 0};
    pacman.cor = YELLOW;
    pacman.direcao_atual = direita;
    pacman.velocidade = 2.0f;

    entidades_para_render[0] = pacman;


    // Fantasma vermelho
    // Perseguidor direto e mais lento
    struct entidade fantasma_vermelho;

    fantasma_vermelho.tipo = entidade_fantasma;
    fantasma_vermelho.pos = (Vector2){0, 0};
    fantasma_vermelho.cor = RED;
    fantasma_vermelho.direcao_atual = esquerda;
    fantasma_vermelho.velocidade = 1.4f;

    entidades_para_render[1] = fantasma_vermelho;


    // Fantasma amarelo
    // Mais rapido, mas possui visao ruim
    struct entidade fantasma_amarelo;

    fantasma_amarelo.tipo = entidade_fantasma;
    fantasma_amarelo.pos = (Vector2){0, 0};
    fantasma_amarelo.cor = GOLD;
    fantasma_amarelo.direcao_atual = direita;
    fantasma_amarelo.velocidade = 1.53f;

    entidades_para_render[2] = fantasma_amarelo;


    // Fantasma verde
    struct entidade fantasma_verde;

    fantasma_verde.tipo = entidade_fantasma;
    fantasma_verde.pos = (Vector2){0, 0};
    fantasma_verde.cor = GREEN;
    fantasma_verde.direcao_atual = cima;
    fantasma_verde.velocidade = 1.5f;

    entidades_para_render[3] = fantasma_verde;


    // Fantasma azul
    struct entidade fantasma_azul;

    fantasma_azul.tipo = entidade_fantasma;
    fantasma_azul.pos = (Vector2){0, 0};
    fantasma_azul.cor = BLUE;
    fantasma_azul.direcao_atual = baixo;
    fantasma_azul.velocidade = 1.5f;

    entidades_para_render[4] = fantasma_azul;


    // Fantasma roxo
    struct entidade fantasma_roxo;

    fantasma_roxo.tipo = entidade_fantasma;
    fantasma_roxo.pos = (Vector2){0, 0};
    fantasma_roxo.cor = PURPLE;
    fantasma_roxo.direcao_atual = esquerda;
    fantasma_roxo.velocidade = 1.5f;

    entidades_para_render[5] = fantasma_roxo;
}
