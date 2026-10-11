#include "grafico_jogo.h"
#include "stdio.h"
// Cores do jogo (Color é uma struct do Raylib)
const Color mapaFundo = { 0, 0, 0, 255 };  
const Color corFundo1 = { 3, 15, 79, 255 };
const Color corFundo2 = { 2, 24, 184, 255 };
const Color corBordas = { 6, 35, 212, 255 };
const Color corDisplay = {12, 40, 250, 255};

void desenhar_parede(int i,int j,int x, int y)
{
    // Força o desenho na borda, caso esteja no limite do mapa.
    Vector2 inicio_borda, fim_borda;
    if (j == 0 || j == tamanho_grid - 1)
    {
        int reajuste_x = (j != 0) * tamanho_bloco;
        inicio_borda.x = x + reajuste_x; inicio_borda.y = y;
        fim_borda.x = x + reajuste_x; fim_borda.y = y + tamanho_bloco;
        
        DrawLineEx(
            inicio_borda,
            fim_borda,
            espessura_parede,
            corBordas
        );
    }
    if(i == 0 || i == tamanho_grid - 1)
    {
        int reajuste_y = (i != 0) * tamanho_bloco;
        inicio_borda.x = x; inicio_borda.y = y + reajuste_y;
        fim_borda.x = x + tamanho_bloco; fim_borda.y = y + reajuste_y;

        DrawLineEx(
            inicio_borda,
            fim_borda,
            espessura_parede,
            corBordas
        );
    }

    /*
    *   Algoritimo para desenhar as bordas de acordo com os vizinhos
    *   --------------------------------------------------------------------
    *   Ele avança a coordenadas relativas ao centro (0,0),(0,1),(0,1),(1,1)
    *   caso a coordenada não seja uma parede, desenha-se uma linha de gros-
    *   sura `espessura_parede`.
    */ 
    for(int dif1 = 0; dif1 <= 1; ++dif1){
        for(int dif2 = 0; dif2 <= 1; ++dif2)
        {
            int vizinho_i = i, vizinho_j = j, v = (2*dif2)-1;
            if(dif1 == 0){vizinho_i += v;} else {vizinho_j += v;}

            // Verifica se o vizinho está dentro do mapa
            if((vizinho_i >= 0 && vizinho_i < tamanho_grid) && (vizinho_j >= 0 && vizinho_j < tamanho_grid))
            {
                enum tipo_bloco tipo_vizinho = mapa_info.mapa[vizinho_i][vizinho_j].tipo;

                // Cancela o desenho caso o vizinho seja uma parede
                if(tipo_vizinho == bloco_parede){continue;}

                if(dif1 == 0)
                {
                    inicio_borda.x = x; inicio_borda.y = y + (dif2*tamanho_bloco);
                    fim_borda.x = x + tamanho_bloco; fim_borda.y = inicio_borda.y;
                }
                else
                {
                    inicio_borda.x = x + (dif2*tamanho_bloco); inicio_borda.y = y;
                    fim_borda.x = inicio_borda.x; fim_borda.y = y + tamanho_bloco;
                }
                
                DrawLineEx(
                    inicio_borda,
                    fim_borda,
                    espessura_parede,
                    corBordas
                );
            }
        }
    }
}

float sprite_atual = 0.0f;
void desenhar_fantasma(struct entidade ent, struct quadros_entidade quadros_fantasma)
{
    if(ent.tipo != entidade_fantasma){return;} // Segurança

    const int total_sprites = quadros_fantasma.quant_quadros * 4;
    int fantasma_sprite_i = (int)(sprite_atual * quadros_fantasma.quant_quadros);

    int p = (int)(tamanho_bloco*0.10);

    Rectangle recBase = {
        0.0f + (fantasma_sprite_i * quadros_fantasma.textura.width/total_sprites),
        0.0f, 
        (float)quadros_fantasma.textura.width/total_sprites, 
        (float)quadros_fantasma.textura.height
    };
    Rectangle recGrafico = {
        ent.pos.x - tamanho_bloco/2 + p,
        ent.pos.y - tamanho_bloco/2 + p,
        (float)tamanho_bloco - p,
        (float)tamanho_bloco - p
    };
    Vector2 posPivo = {0,0};
    
    DrawTexturePro(quadros_fantasma.textura, recBase, recGrafico, posPivo, 0.0f, WHITE);
}
void desenhar_pacman(struct entidade ent, struct quadros_entidade quadros_pacman)
{
    if(ent.tipo != entidade_pacman){return;} // Segurança
    const int total_sprites = quadros_pacman.quant_quadros * 4;
    int pacman_sprite_i = (int)(sprite_atual * quadros_pacman.quant_quadros);

    int p = (int)(tamanho_bloco*0.30);

    Rectangle recBase = {
        0.0f + (pacman_sprite_i * quadros_pacman.textura.width/total_sprites),
        0.0f, 
        (float)quadros_pacman.textura.width/total_sprites, 
        (float)quadros_pacman.textura.height
    };
    Rectangle recGrafico = {
        ent.pos.x - tamanho_bloco/2 + p,
        ent.pos.y - tamanho_bloco/2 + p,
        (float)tamanho_bloco - p,
        (float)tamanho_bloco - p
    };
    Vector2 posPivo = {0,0};
    
    DrawTexturePro(quadros_pacman.textura, recBase, recGrafico, posPivo, 0.0f, WHITE);
}

// Variáveis para a animação dos pontos
float multi_alpha = 1.0;
int sentido_alpha = -1;
Vector2 pos_spawn_fantasmas = {0,0},  pos_spawn_jogador = {0,0};

void desenhar_mapa() // Desenha o mapa com entidades
{
    int x_mapa = (largura_tela-tamanho_mapa)-distancia_mapa, y_mapa = (altura_tela-tamanho_mapa)/2;
    DrawRectangle(
        x_mapa,
        y_mapa,
        tamanho_mapa,
        tamanho_mapa,
        mapaFundo
    );

    int eixo_x = x_mapa, eixo_y = y_mapa;
    for (int i = 0; i < tamanho_grid; ++i) 
    {
        for (int j = 0; j < tamanho_grid; ++j)
        {
            // Determina o desenho a partir do tipo do bloco analisado
            switch (mapa_info.mapa[i][j].tipo)
            {
            case bloco_parede:
                desenhar_parede(
                    i,
                    j,
                    eixo_x,
                    eixo_y
                );
                break;

            case bloco_area_com_pontos:
                const Color cor_ponto = {255,255,255,(int)(255*multi_alpha)};
                DrawCircle(
                    eixo_x+tamanho_bloco/2,
                    eixo_y+tamanho_bloco/2,
                    tamanho_bloco/10,
                    cor_ponto
                );
                break;

            case bloco_s_jogador:
                int xj = eixo_x+tamanho_bloco/2, yj = eixo_y+tamanho_bloco/2;

                if(pos_spawn_jogador.x == 0)
                {
                    pos_spawn_jogador.x = xj;
                    pos_spawn_jogador.y = yj;
                }

                DrawCircle(
                    xj,
                    yj,
                    tamanho_bloco/10,
                    BLUE
                );
                break;

            case bloco_s_fantasmas:
                int xf = eixo_x+tamanho_bloco/2, yf = eixo_y+tamanho_bloco/2; 

                if(pos_spawn_fantasmas.x == 0)
                {
                    pos_spawn_fantasmas.x = xf;
                    pos_spawn_fantasmas.y = yf;
                }

                DrawCircle(
                    xf,
                    yf,
                    tamanho_bloco/10,
                    PURPLE
                );
                break;

            default:
                break;
            }
            eixo_x += tamanho_bloco; // desloca a coordenada (x) para o proximo desenho
        }
        eixo_y += tamanho_bloco; // desloca a coordenada (y) para o proximo desenho
        eixo_x = x_mapa; // Reseta a coordenada (x) para o inicio do mapa


    }
}

void desenhar_entidades(struct quadros_entidade quadros_pacman, struct quadros_entidade quadros_fantasma)
{
    //Desenhando entidades
    for(int i = 0; i < quantidade_entidades; ++i)
    {
        struct entidade ent = entidades_para_render[i];
        
        switch (ent.tipo)
        {
        case entidade_fantasma:
            ent.pos.x = ent.pos.x == 0 ? pos_spawn_fantasmas.x : ent.pos.x;
            ent.pos.y = ent.pos.y == 0 ? pos_spawn_fantasmas.y : ent.pos.y;
        
            desenhar_fantasma(ent, quadros_fantasma);
            break;
        case entidade_pacman:
            ent.pos.x = ent.pos.x == 0 ? pos_spawn_jogador.x : ent.pos.x;
            ent.pos.y = ent.pos.y == 0 ? pos_spawn_jogador.y : ent.pos.y;

            desenhar_pacman(ent, quadros_pacman);
            break;
        }
    }
}

void desenhar_display()
{
    int display_x = distancia_mapa, display_y = 55;
    int tamanho_x = tamanho_display,tamanho_y = tamanho_display;
    int reajuste_x = 10, reajuste_y = 15;
    DrawRectangle(
        display_x,
        display_y,
        tamanho_x,
        tamanho_y,
        mapaFundo
    );
    
    Vector2 i,f;
    i.x = display_x; i.y = display_y; 
    f.x = display_x+tamanho_display; f.y = display_y; 
    DrawLineEx(
        i,
        f,
        5,
        corDisplay
    );

    i.x = display_x; i.y = display_y+tamanho_y; 
    f.x = display_x+tamanho_display; f.y = display_y+tamanho_y; 
    DrawLineEx(
        i,
        f,
        5,
        corDisplay
    );

    i.x = display_x; i.y = display_y; 
    f.x = display_x; f.y = display_y+tamanho_y; 
    DrawLineEx(
        i,
        f,
        5,
        corDisplay
    );

    i.x = display_x+tamanho_display; i.y = display_y; 
    f.x = display_x+tamanho_display; f.y = display_y+tamanho_y; 
    DrawLineEx(
        i,
        f,
        5,
        corDisplay
    );

    DrawText(
        "Info Jogador ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        corDisplay
    );
    reajuste_y += 25;
    DrawText(
        "Pontos restantes: ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        WHITE
    );
    DrawText(
        TextFormat("%d",mapa_info.total_pontos),
        display_x+tamanho_x-MeasureText("888",15)-10,
        display_y+reajuste_y,
        15,
        WHITE
    );
    reajuste_y += 30;
    DrawText(
        "Vidas: ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        WHITE);
    DrawText(
        TextFormat("%d",3),
        display_x+tamanho_x-MeasureText("8",15)-10,
        display_y+reajuste_y,
        15,
        WHITE
    );
    
    reajuste_y += 35;
    DrawRectangle(
        display_x+reajuste_x,
        display_y+reajuste_y,
        tamanho_display-20,
        3,
        corDisplay
    );

    reajuste_y += 15;
    DrawText(
        "Info Mapa ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        corDisplay
    );
    reajuste_y += 25;
    DrawText(
        "Índice do Mapa: ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        WHITE
    );
    DrawText(
        TextFormat("%d",mapa_info.indice_escolhido),
        display_x+tamanho_x-MeasureText("8",15)-10,
        display_y+reajuste_y,
        15,
        WHITE
    );
    reajuste_y += 30;
    DrawText(
        "Total de Pontos: ",
        display_x+reajuste_x,
        display_y+reajuste_y,
        15,
        WHITE
    );
    DrawText(
        TextFormat("%d",mapa_info.total_pontos),
        display_x+tamanho_x-MeasureText("888",15)-10,
        display_y+reajuste_y,
        15,
        WHITE
    );
}

enum grafico_jogo iniciar_grafico()
{

    InitWindow(largura_tela, altura_tela, "PacMan - Trabalho Contrução de Algoritimos");
    SetTargetFPS(FPS);

    struct quadros_entidade quadros_pacman = {
        LoadTexture("src/sprite_pacman.png"),
        4
    };
    struct quadros_entidade quadros_fantasmas = {
        LoadTexture("src/sprite_fantasma.png"),
        2
    };

    while (!WindowShouldClose())
    {
        // Animação dos pontos no mapa.
        multi_alpha += (1.0/FPS) * sentido_alpha;

        if(multi_alpha < 0.375f){multi_alpha = 0.375f; sentido_alpha=1;}
        if(multi_alpha > 1.0f){multi_alpha = 1.0f; sentido_alpha=-1;}

        // Sprite
        sprite_atual += GetFrameTime();
        if(sprite_atual > 1){sprite_atual = 0;}

        BeginDrawing();
            ClearBackground(corFundo1);
            DrawRectangleGradientH(0,0,largura_tela,altura_tela,corFundo1,corFundo2);
            DrawText(
                "Pacman",
                20,
                10,
                40,
                WHITE
            );
            DrawText(
                "Construção de Algoritimos",
                20+MeasureText("Pacman",40)+10,
                30,
                15,
                WHITE
            );
            DrawText(
                "Erick Alves e Kauã Soares",
                20,
                altura_tela-30,
                15,
                WHITE
            );

            if(!mapa_escolhido){return retorno_falha_mapa;}

            desenhar_mapa();
            desenhar_display();
            desenhar_entidades(quadros_pacman, quadros_fantasmas);
        EndDrawing();
    }

    UnloadTexture(quadros_pacman.textura);
    UnloadTexture(quadros_fantasmas.textura);
    CloseWindow();

    return retorno_sair;
}