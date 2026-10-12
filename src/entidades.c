#include "entidades.h"
#include "geranciador_de_mapa.h"

struct entidade entidades_para_render[quantidade_entidades];

struct entidade gerar_fantasma(Color cor,enum direcao d, float velocidade)
{
    Image fantasma_sprite = LoadImage("src/sprite_fantasma.png");
    const Color CorEsboco = {221, 33, 41};

    struct entidade fantasma;
    fantasma.tipo = entidade_fantasma;
    fantasma.pos = (Vector2){0, 0};
    fantasma.cor = cor;
    fantasma.direcao_atual = d;
    fantasma.velocidade = velocidade;

    ImageColorReplace(&fantasma_sprite, RED, cor);
    struct quadros_entidade quadros = {
        LoadTextureFromImage(fantasma_sprite),
        2
    };
    SetTextureFilter(quadros.textura, TEXTURE_FILTER_POINT);
    fantasma.sprites = quadros;
    
    UnloadImage(fantasma_sprite);
    return fantasma;
}

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
    struct quadros_entidade quadros_pacman = {
        LoadTexture("src/sprite_pacman.png"),
        4
    };
    pacman.sprites = quadros_pacman;
    entidades_para_render[0] = pacman;

    // Fantasma vermelho
    // Perseguidor direto e mais lento
    entidades_para_render[1] = gerar_fantasma(RED, esquerda, 1.4f);
    // Fantasma amarelo
    // Mais rapido, mas possui visao ruim
    entidades_para_render[2] = gerar_fantasma(GOLD, direita, 1.53f);
    // Fantasma verde
    entidades_para_render[3] = gerar_fantasma(GREEN, cima, 1.5f);
    // Fantasma azul
    entidades_para_render[4] = gerar_fantasma(BLUE, baixo, 1.5f);
    // Fantasma roxo
    entidades_para_render[5] = gerar_fantasma(PURPLE, esquerda, 1.5f);
}

void descarregar_entidades(void)
{
    for(int i = 0; i < quantidade_entidades; i++)
    {
        UnloadTexture(entidades_para_render[i].sprites.textura);
    }
}