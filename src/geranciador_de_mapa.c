/*
Gerenciador de Mapa
------------------------------------------
Módulo responsavel pelo gerenciamento do mapa, gerindo o arquivo mapas.txt, salvando pontos de interesse,
escolha do mapa, etc...
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "geranciador_de_mapa.h"

// Definidos por (y,x)

FILE *arquivo_ptr = NULL;
int mapa_escolhido = 0;
struct Mapa mapa_info;

int iniciar_data_mapas()
{
    arquivo_ptr = fopen(caminho,"r");
    if(arquivo_ptr == NULL){return 0;}
    mapa_info.total_pontos = 0;

    return 1;
}

int quantidade_de_mapas()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(quantidade)] - Mapas nao foram incializados!"); return 0;}
    int quantidade = 0;
    fseek(arquivo_ptr, 0, SEEK_SET); // Move o ponteiro para o inicio

    char linha[tamanho_grid];
    while (fgets(linha,sizeof(linha),arquivo_ptr) != NULL)
    {
        if(linha[0] == 'M')
        {
            ++quantidade;
            continue;
        }
    }
    
    return quantidade;
}

// Escolhe um mapa de forma aleatória e define `mapa_info`
int escolher_mapa()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(quantidade)] - Mapas nao foram incializados!"); return 0;}

    int quant = quantidade_de_mapas();

    srand(time(NULL)); // Define uma seed de geração de acordo com o horario (Para aumentar a randomização)
    int escolhido = rand() % quant + 1; 
    printf("%d",escolhido);
    fseek(arquivo_ptr, 0, SEEK_SET); // Retorna o ponteiro ao inicio do arquivo

    char c = fgetc(arquivo_ptr);

    int m = 0, i = 0, j = 0;
    while ((m != escolhido) || (c != '!')) // Repete ate achar "!" do 'm' alvo.
    {
        if(c == 'M')
        {
            ++m;
        }
        
        if(m == escolhido && (c == '.' || c == '#' || c == 'j' || c == 'f' || c == '-'))
        {
            struct bloco b;
            switch (c)
            {
            case '.':
                b.tipo = bloco_area_com_pontos;
                ++mapa_info.total_pontos;
                break;
            case '#':
                b.tipo = bloco_parede;
                break;
            case 'j':
                b.tipo = bloco_s_jogador;
                mapa_info.spawn_jogador[0] = i;
                mapa_info.spawn_jogador[1] = j;
                break;
            case 'f':
                b.tipo = bloco_s_fantasmas;
                mapa_info.spawn_fantasmas[0] = i;
                mapa_info.spawn_fantasmas[1] = j;
                break;
            case '-':
                b.tipo = area_vazia;
                break;
            }
            mapa_info.mapa[i][j] = b;
  
            ++j;
            if(j == tamanho_grid){j = 0; ++i;}
        }
        c = fgetc(arquivo_ptr);
    }
    mapa_escolhido = 1;
    return 1;
}

int fechar_data_mapas()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(fechar)] - Mapas não foram incializados!"); return 0;}
    fclose(arquivo_ptr);
    return 1;
}