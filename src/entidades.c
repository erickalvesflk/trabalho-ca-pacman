#include "entidades.h"
#include "geranciador_de_mapa.h"

struct entidade entidades_para_render[quantidade_entidades];

void gerar_entidades()
{
    for(int i = 0; i < quantidade_entidades; ++i)
    {entidades_para_render[i].tipo = entidade_vazia;}

    struct entidade pacman;
    pacman.tipo = entidade_pacman;

    Vector2 pacman_pos = {0,0}; // Coloca (0,0) como o spawn, o grafico ja reajusta a pos. (apenas se definir 0)
    pacman.pos = pacman_pos; 

    Color pacman_cor = YELLOW;
    pacman.cor = pacman_cor; 

    entidades_para_render[0] = pacman;

    struct entidade fantasma1;
    fantasma1.tipo = entidade_fantasma;

    Vector2 fantasma_pos = {0,0}; // Coloca (0,0) como o spawn, o grafico ja reajusta a pos. (apenas se definir 0)
    fantasma1.pos = fantasma_pos; 

    Color fantasma_cor = PINK;
    fantasma1.cor = fantasma_cor; 

    entidades_para_render[1] = fantasma1;
}