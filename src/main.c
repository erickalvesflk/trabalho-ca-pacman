/*
TRABALHO PACMAN - Contrução de algoritimos UERJ 2026.1
------------------------------------------------------
    Trabalho onde Recriamos o jogo clássico PACMAN utilizando apenas a linguagem C
    e a biblioteca de criação de jogos RAYLIB. Em geral, Kauã Soares foi responável
    pelo núcleo do jogo (Sistemas dos fantasmas, pacman e pontos) e Erick Alves foi
    responsável pela parte gráfica (Criação e Gerenciamento de Mapas, Sprites e Gráfico)

    Professor: Eugênio Silva
    Participantes:
    - Erick Alves dos Santos
    - Kauã Soares dos Santos Rodrigues

    Aproveite o Jogo!
*/
#include <stdio.h>
#include "raylib.h"
#include "geranciador_de_mapa.h"
#include "entidades.h"
#include "grafico_jogo.h"

int main()
{
    // Inicialização da Janela
    InitWindow(
        largura_tela,
        altura_tela,
        "PacMan - Trabalho Construcao de Algoritmos"
    );
    // Inserindo Icone
    const Image icone = LoadImage("src/icone_pacman.png");
    SetWindowIcon(icone);
    UnloadImage(icone);

    SetTargetFPS(FPS);


    enum grafico_jogo motivo_retorno = retorno_falha_mapa;
    do
    {
        int sucesso_carregar_data = iniciar_data_mapas();
        if(sucesso_carregar_data == 0){break;}

        escolher_mapa();
        fechar_data_mapas();

        motivo_retorno = iniciar_grafico();
    } while (!(motivo_retorno == retorno_sair || motivo_retorno == retorno_falha_mapa));

    CloseWindow();

    return 0;
}
