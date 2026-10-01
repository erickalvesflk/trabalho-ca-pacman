#include <stdio.h>
#include "raylib.h"
#include "geranciador_de_mapa.h"
#include "grafico_jogo.h"

int main()
{
    puts("Trabalho pacman");
    int r = iniciar_data_mapas();
    
    escolher_mapa();
    fechar_data_mapas();
    printf("Quantidade total de pontos: %d",mapa_info.total_pontos);

    iniciar_grafico();

    return 0;
}