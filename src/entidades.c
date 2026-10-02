#include "entidades.h"
#include "geranciador_de_mapa.h"

struct entidade entidades_para_render[quantidade_entidades];

void gerar_entidades()
{
    for(int i = 0; i < quantidade_entidades; ++i)
    {
        entidades_para_render[i].tipo = entidade_vazia;
    }

    // Pacman
    struct entidade pacman;

    pacman.tipo = entidade_pacman;
    pacman.pos = (Vector2){0, 0};
    pacman.cor = YELLOW;
    pacman.direcao_atual = direita;
    pacman.velocidade = 2.0f;

    entidades_para_render[0] = pacman;


    // Primeiro fantasma
    struct entidade fantasma1;

    fantasma1.tipo = entidade_fantasma;
    fantasma1.pos = (Vector2){0, 0};
    fantasma1.cor = PINK;
    fantasma1.direcao_atual = esquerda;
    fantasma1.velocidade = 1.5f;

    entidades_para_render[1] = fantasma1;
}
