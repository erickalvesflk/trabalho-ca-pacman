/*
Gerenciador de Mapa
------------------------------------------
Módulo responsavel pelo gerenciamento do mapa, gerindo o arquivo mapas.txt, salvando pontos de interesse,
escolha do mapa, etc...
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define caminho "mapas.txt"
#define L 14

// Definidos por (y,x)
struct Mapa
{
    char mapa[L][L];
    int spawn_jogador[2];
    int spawn_fantasmas[2];
};

FILE *arquivo_ptr = NULL;
struct Mapa mapa_obj;

int iniciar_data_mapas()
{
    arquivo_ptr = fopen(caminho,"r");
    if(arquivo_ptr == NULL){return 0;}

    return 1;
}

int quantidade_de_mapas()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(quantidade)] - Mapas nao foram incializados!"); return 0;}
    int quantidade = 0;
    fseek(arquivo_ptr, 0, SEEK_SET); // Move o ponteiro para o inicio

    char linha[L];
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

int escolher_mapa()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(quantidade)] - Mapas nao foram incializados!"); return 0;}

    int quant = quantidade_de_mapas();

    srand(time(NULL)); // Define uma seed de geração de acordo com o horario (Para aumentar a randomização)
    int escolhido = rand() % quant + 1; 

    fseek(arquivo_ptr, 0, SEEK_SET); // Retorna o ponteiro ao inicio do arquivo

    char c = fgetc(arquivo_ptr);

    int m = 0, i = 0, j = 0;
    while ((m != escolhido) || (c != '!')) // Repete ate achar "!" do 'm' alvo.
    {
        if(c == 'M')
        {
            ++m;
        }
        if(m == escolhido && (c == '.' || c == '#' || c == 'j' || c == 'f'))
        {
            mapa_obj.mapa[i][j] = c;
            if(c == 'j') // Salva o spawn do jogador e do fantasma
            {
                mapa_obj.spawn_jogador[0] = i;
                mapa_obj.spawn_jogador[1] = j;
            }
            else if (c == 'f')
            {
                mapa_obj.spawn_fantasmas[0] = i;
                mapa_obj.spawn_fantasmas[1] = j;
            }
            ++j;
            if(j == L){j = 0; ++i;}
        }
        c = fgetc(arquivo_ptr);
    }    
    return 1;
}

int fechar_data_mapas()
{
    if(arquivo_ptr == NULL){ perror("[Gerenciador de Mapas(fechar)] - Mapas não foram incializados!"); return 0;}
    fclose(arquivo_ptr);
    return 1;
}