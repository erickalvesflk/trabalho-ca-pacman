#include "grafico_jogo.h"

// Cores do jogo
const Color mapaFundo = { 0, 0, 0, 255 };  
const Color corFundo1 = { 3, 15, 79, 255 };
const Color corFundo2 = { 2, 24, 184, 255 };
const Color corBordas = { 6, 35, 212, 255 };
const Color corDisplay = {12, 40, 250, 255};

void desenhar_parede(int i,int j,int x, int y)
{
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

void desenhar_fantasma(struct entidade ent)
{
    if(ent.tipo != entidade_fantasma){return;} // Segurança
    int t = (int)(tamanho_bloco/1.5);
    DrawRectangle(ent.pos.x-t/2,ent.pos.y-t/2,t,t, ent.cor);
}

void desenhar_pacman(struct entidade ent)
{
    if(ent.tipo != entidade_pacman){return;} // Segurança
    int t = (int)(tamanho_bloco/2.5);
    DrawCircle(ent.pos.x,ent.pos.y,t,ent.cor);
}

// Variáveis para a animação dos pontos
float multi_alpha = 1.0;
int sentido_alpha = -1;
Vector2 pos_spawn_fantasmas = {0,0},  pos_spawn_jogador = {0,0};

void desenhar_mapa() // Desenha o mapa com entidades
{
    int x_mapa = (largura_tela-tamanho_mapa)-distancia_mapa, y_mapa = (altura_tela-tamanho_mapa)/2;
    DrawRectangle(x_mapa,y_mapa,tamanho_mapa,tamanho_mapa,mapaFundo);

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
                int xj = eixo_x+tamanho_bloco/2, yj = eixo_y+tamanho_bloco/2;

                if(pos_spawn_jogador.x == 0)
                {
                    pos_spawn_jogador.x = xj;
                    pos_spawn_jogador.y = yj;
                }

                DrawCircle(xj+tamanho_bloco/2,yj+tamanho_bloco/2,tamanho_bloco/10,BLUE);
                break;
            case bloco_s_fantasmas:
                int xf = eixo_x+tamanho_bloco/2, yf = eixo_y+tamanho_bloco/2; 

                if(pos_spawn_fantasmas.x == 0)
                {
                    pos_spawn_fantasmas.x = xf;
                    pos_spawn_fantasmas.y = yf;
                }

                DrawCircle(xf+tamanho_bloco/2,yf+tamanho_bloco/2,tamanho_bloco/10,RED);
                break;
            default:
                break;
            }
            eixo_x += tamanho_bloco;
        }
        eixo_y += tamanho_bloco;
        eixo_x = x_mapa;

        //Desenhando entidades
        for(int i = 0; i < quantidade_entidades; ++i)
        {
            struct entidade ent = entidades_para_render[i];
            
            switch (ent.tipo)
            {
            case entidade_fantasma:
                ent.pos.x = ent.pos.x == 0 ? pos_spawn_fantasmas.x : ent.pos.x;
                ent.pos.y = ent.pos.y == 0 ? pos_spawn_fantasmas.y : ent.pos.y;
            
            desenhar_fantasma(ent);
            break;
            case entidade_pacman:
                ent.pos.x = ent.pos.x == 0 ? pos_spawn_jogador.x : ent.pos.x;
                ent.pos.y = ent.pos.y == 0 ? pos_spawn_jogador.y : ent.pos.y;

                desenhar_pacman(ent);
                break;
            }
        }
    }
}

void desenhar_display()
{
    int display_x = distancia_mapa, display_y = 55;
    int tamanho_x = tamanho_display,tamanho_y = tamanho_mapa;
    int reajuste_x = 10, reajuste_y = 15;
    DrawRectangle(display_x,display_y,tamanho_x,tamanho_y,mapaFundo);
    
    Vector2 i,f;
    i.x = display_x; i.y = display_y; 
    f.x = display_x+tamanho_display; f.y = display_y; 
    DrawLineEx(i,f,5,corDisplay);
    i.x = display_x; i.y = display_y+tamanho_y; 
    f.x = display_x+tamanho_display; f.y = display_y+tamanho_y; 
    DrawLineEx(i,f,5,corDisplay);
    i.x = display_x; i.y = display_y; 
    f.x = display_x; f.y = display_y+tamanho_y; 
    DrawLineEx(i,f,5,corDisplay);
    i.x = display_x+tamanho_display; i.y = display_y; 
    f.x = display_x+tamanho_display; f.y = display_y+tamanho_y; 
    DrawLineEx(i,f,5,corDisplay);

    DrawText("Info Jogador ",display_x+reajuste_x,display_y+reajuste_y,15,corDisplay);
    reajuste_y += 25;
    DrawText("Pontos restantes: ",display_x+reajuste_x,display_y+reajuste_y,15,WHITE);
    DrawText(TextFormat("%d",mapa_info.total_pontos),display_x+tamanho_x-MeasureText("888",15)-10,display_y+reajuste_y,15,WHITE);
    reajuste_y += 30;
    DrawText("Vidas: ",display_x+reajuste_x,display_y+reajuste_y,15,WHITE);
    DrawText(TextFormat("%d",3),display_x+tamanho_x-MeasureText("8",15)-10,display_y+reajuste_y,15,WHITE);
    
    reajuste_y += 35;
    DrawRectangle(display_x+reajuste_x,display_y+reajuste_y,tamanho_display-20,3,corDisplay);

    reajuste_y += 15;
    DrawText("Info Mapa ",display_x+reajuste_x,display_y+reajuste_y,15,corDisplay);
    reajuste_y += 25;
    DrawText("Índice do Mapa: ",display_x+reajuste_x,display_y+reajuste_y,15,WHITE);
    DrawText(TextFormat("%d",mapa_info.indice_escolhido),display_x+tamanho_x-MeasureText("8",15)-10,display_y+reajuste_y,15,WHITE);
    reajuste_y += 30;
    DrawText("Total de Pontos: ",display_x+reajuste_x,display_y+reajuste_y,15,WHITE);
    DrawText(TextFormat("%d",mapa_info.total_pontos),display_x+tamanho_x-MeasureText("888",15)-10,display_y+reajuste_y,15,WHITE);

    reajuste_y += 30;
    DrawRectangle(display_x+reajuste_x,display_y+reajuste_y,tamanho_display-20,3,corDisplay);

    reajuste_y += 15;
    DrawText("Ranking ",display_x+reajuste_x,display_y+reajuste_y,15,corDisplay);
}

void iniciar_grafico()
{

    InitWindow(largura_tela, altura_tela, "PacMan - Trabalho Contrução de Algoritimos");
    SetTargetFPS(FPS);

    while (!WindowShouldClose())
    {
        // Animação dos pontos no mapa.
        multi_alpha += (1.0/(FPS*1)) * sentido_alpha;

        if(multi_alpha < 0.375f){multi_alpha = 0.375f; sentido_alpha=1;}
        if(multi_alpha > 1.0f){multi_alpha = 1.0f; sentido_alpha=-1;}

        BeginDrawing();
            ClearBackground(corFundo1);
            DrawRectangleGradientH(0,0,largura_tela,altura_tela,corFundo1,corFundo2);
            DrawText("Pacman",20,10,40,WHITE);
            DrawText("Construção de Algoritimos",20+MeasureText("Pacman",40)+10,30,15,WHITE);
            DrawText("Erick Alves e Kauã Soares",20,altura_tela-30,15,WHITE);

            if(!mapa_escolhido){return;}

            desenhar_mapa();
            desenhar_display();
        EndDrawing();
    }
    CloseWindow();
}
