#include "grafico_jogo.h"
#include <stdio.h>
void desenhar_parede(int i,int j,int x, int y)
{
    const Color corBordas = { 6, 35, 212, 255 };
    if (j == 0)
    {
        Vector2 inicio_borda = {x, y}, fim_borda = {x, y + tamanho_bloco};   
        DrawLineEx(inicio_borda,fim_borda,espessura_parede,corBordas);   
    }
    if( i == 0)
    {
        Vector2 inicio_borda = {x, y}, fim_borda = {x + tamanho_bloco, y}; 
        DrawLineEx(inicio_borda,fim_borda,espessura_parede,corBordas);     
    }
    for(int dif1 = 0; dif1 <= 1; ++dif1){
        for(int dif2 = 0; dif2 <= 1; ++dif2)
        {
            int vizinho_i = i, vizinho_j = j, v = (2*dif2)-1;
            if(dif1 == 0)
            {vizinho_i += v;}
            else
            {vizinho_j += v;}

            if((vizinho_i >= 0 && vizinho_i <= tamanho_grid) && (vizinho_j >= 0 && vizinho_j <= tamanho_grid))
            {

                enum tipo_bloco tipo_vizinho = mapa_info.mapa[vizinho_i][vizinho_j].tipo;
                if(( (vizinho_j+1 <= tamanho_grid) && tipo_vizinho == bloco_parede) ){continue;} 
                
                

                Vector2 inicio_borda, fim_borda;
                if(dif1 == 0)
                {
                    inicio_borda.x = x;
                    inicio_borda.y = y + (dif2*tamanho_bloco);
                    fim_borda.x = x + tamanho_bloco;
                    fim_borda.y = inicio_borda.y;
                }
                else
                {
                    inicio_borda.x = x + (dif2*tamanho_bloco);
                    inicio_borda.y = y;

                    fim_borda.x = inicio_borda.x;
                    fim_borda.y = y + tamanho_bloco;
                }

                DrawLineEx(inicio_borda,fim_borda,espessura_parede,corBordas);

            }
        }
    }
}

void iniciar_grafico()
{
    const Color corFundo = { 3, 15, 79, 255 };
    const Color mapaFundo = { 0, 0, 0, 255 };  

    InitWindow(largura_tela, altura_tela, "PacMan - Trabalho Contrução de Algoritimos");
    SetTargetFPS(FPS);

    float multi_alpha = 1.0;
    int sentido_alpha = -1;
    while (!WindowShouldClose())
    {
        // Animação dos pontos no mapa.
        multi_alpha += (1.0/(FPS*1)) * sentido_alpha;

        if(multi_alpha < 0.375f){multi_alpha = 0.375f; sentido_alpha=1;}
        if(multi_alpha > 1.0f){multi_alpha = 1.0f; sentido_alpha=-1;}

        BeginDrawing();
            ClearBackground(corFundo);
            DrawText("Pacman",20,10,40,WHITE);
            DrawText("Construção de Algoritimos - Erick Alves e Kauã Soares",20,altura_tela-30,15,WHITE);
            int x_mapa = (largura_tela-tamanho_mapa)-distancia_mapa, y_mapa = (altura_tela-tamanho_mapa)/2;
            DrawRectangle(x_mapa,y_mapa,tamanho_mapa,tamanho_mapa,mapaFundo);

            
            if(mapa_escolhido)
            {
                int eixo_x = x_mapa, eixo_y = y_mapa;
                for (int i = 0; i < tamanho_grid; ++i)
                {
                    for (int j = 0; j < tamanho_grid; ++j)
                    {
                        switch (mapa_info.mapa[i][j].tipo)
                        {
                        case bloco_parede:
                            desenhar_parede(i,j,eixo_x,eixo_y);
                            break;
                        case bloco_area_com_pontos:
                            const Color cor_ponto = {255,255,255,(int)(255*multi_alpha)};
                            DrawCircle(eixo_x+tamanho_bloco/2,eixo_y+tamanho_bloco/2,tamanho_bloco/10,cor_ponto);
                            break;
                        case bloco_s_jogador:
                            DrawCircle(eixo_x+tamanho_bloco/2,eixo_y+tamanho_bloco/2,tamanho_bloco/10,corFundo);
                            break;
                        case bloco_s_fantasmas:
                            DrawCircle(eixo_x+tamanho_bloco/2,eixo_y+tamanho_bloco/2,tamanho_bloco/10,RED);
                            break;
                        default:
                            break;
                        }
                        eixo_x += tamanho_bloco;
                    }
                    eixo_y += tamanho_bloco;
                    eixo_x = x_mapa;
                }
            }
        EndDrawing();
    }
    CloseWindow();
}
