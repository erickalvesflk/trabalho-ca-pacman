#include <stdio.h>
#include "raylib.h"
#include "geranciador_de_mapa.h"
#include "entidades.h"
#include "grafico_jogo.h"

int main()
{
    enum grafico_jogo motivo_retorno;
    do
    {
        int sucesso_carregar_data = iniciar_data_mapas();
        if(sucesso_carregar_data == 0){break;}

        escolher_mapa();
        fechar_data_mapas();

        motivo_retorno = iniciar_grafico();

    } while (!(motivo_retorno == retorno_sair || motivo_retorno == retorno_falha_mapa));
    
    return 0;
}