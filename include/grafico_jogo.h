/*
Gráficos
------------------------------------------
Módulo responsavel pelo criação e funcionamento do gráfico, criando toda a parte visual
do jogo.
*/
#include "raylib.h"
#include "entidades.h"
#include "geranciador_de_mapa.h"

#define largura_tela 750
#define altura_tela (int)(largura_tela - (largura_tela*0.25))
#define distancia_mapa 20
#define FPS 30

#define espessura_parede 5
#define tamanho_mapa 450
#define tamanho_bloco (tamanho_mapa/tamanho_grid)
#define tamanho_display tamanho_mapa/2

enum grafico_jogo // Possiveis retornos do grafico
{
    retorno_sair,
    retorno_derrota,
    retorno_vitoria,
    retorno_falha_mapa
};


enum grafico_jogo iniciar_grafico(void); // Roda o gráfico do jogo