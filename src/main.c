#include <stdio.h>
#include "geranciador_de_mapa.h"

int main()
{
    puts("Trabalho pacman");
    int r = iniciar_data_mapas();
    
    escolher_mapa();
    fechar_data_mapas();

    return 0;
}