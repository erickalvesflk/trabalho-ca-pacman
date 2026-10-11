#include "grafico_jogo.h"
#include "fantasmas.h"
#include "pacman.h"

// Cores do jogo
const Color mapaFundo = { 0, 0, 0, 255 };  
const Color corFundo = { 3, 15, 79, 255 };
const Color corBordas = { 6, 35, 212, 255 };

void desenhar_parede(int i, int j, int x, int y)
{
    if(j == 0)
    {
        Vector2 inicio_borda = {x, y};
        Vector2 fim_borda = {x, y + tamanho_bloco};

        DrawLineEx(inicio_borda, fim_borda, espessura_parede, corBordas);
    }

    if(i == 0)
    {
        Vector2 inicio_borda = {x, y};
        Vector2 fim_borda = {x + tamanho_bloco, y};

        DrawLineEx(inicio_borda, fim_borda, espessura_parede, corBordas);
    }

    for(int dif1 = 0; dif1 <= 1; ++dif1)
    {
        for(int dif2 = 0; dif2 <= 1; ++dif2)
        {
            int vizinho_i = i;
            int vizinho_j = j;
            int v = (2 * dif2) - 1;

            if(dif1 == 0)
            {
                vizinho_i += v;
            }
            else
            {
                vizinho_j += v;
            }

            if(vizinho_i >= 0 && vizinho_i < tamanho_grid &&
               vizinho_j >= 0 && vizinho_j < tamanho_grid)
            {
                enum tipo_bloco tipo_vizinho =
                    mapa_info.mapa[vizinho_i][vizinho_j].tipo;

                if(tipo_vizinho == bloco_parede)
                {
                    continue;
                }

                Vector2 inicio_borda;
                Vector2 fim_borda;

                if(dif1 == 0)
                {
                    inicio_borda.x = x;
                    inicio_borda.y = y + (dif2 * tamanho_bloco);

                    fim_borda.x = x + tamanho_bloco;
                    fim_borda.y = inicio_borda.y;
                }
                else
                {
                    inicio_borda.x = x + (dif2 * tamanho_bloco);
                    inicio_borda.y = y;

                    fim_borda.x = inicio_borda.x;
                    fim_borda.y = y + tamanho_bloco;
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

void desenhar_fantasma(struct entidade ent)
{
    if(ent.tipo != entidade_fantasma)
    {
        return;
    }

    int t = (int)(tamanho_bloco / 1.5);

    DrawRectangle(
        ent.pos.x - t / 2,
        ent.pos.y - t / 2,
        t,
        t,
        ent.cor
    );
}

void desenhar_pacman(struct entidade ent)
{
    if(ent.tipo != entidade_pacman)
    {
        return;
    }

    int t = (int)(tamanho_bloco / 2.5);

    DrawCircle(
        ent.pos.x,
        ent.pos.y,
        t,
        ent.cor
    );
}


// Variaveis para a animacao dos pontos
float multi_alpha = 1.0;
int sentido_alpha = -1;

Vector2 pos_spawn_fantasmas = {0, 0};
Vector2 pos_spawn_jogador = {0, 0};

// Indica se as entidades ja foram colocadas no spawn
int entidades_inicializadas = 0;


void desenhar_mapa()
{
    int x_mapa = (largura_tela - tamanho_mapa) - distancia_mapa;
    int y_mapa = (altura_tela - tamanho_mapa) / 2;

    DrawRectangle(
        x_mapa,
        y_mapa,
        tamanho_mapa,
        tamanho_mapa,
        mapaFundo
    );

    int eixo_x = x_mapa;
    int eixo_y = y_mapa;

    // Desenha os blocos do mapa
    for(int i = 0; i < tamanho_grid; ++i)
    {
        for(int j = 0; j < tamanho_grid; ++j)
        {
            switch(mapa_info.mapa[i][j].tipo)
            {
                case bloco_parede:
                    desenhar_parede(i, j, eixo_x, eixo_y);
                    break;

                case bloco_area_com_pontos:
                {
                    Color cor_ponto =
                    {
                        255,
                        255,
                        255,
                        (int)(255 * multi_alpha)
                    };

                    DrawCircle(
                        eixo_x + tamanho_bloco / 2,
                        eixo_y + tamanho_bloco / 2,
                        tamanho_bloco / 10,
                        cor_ponto
                    );

                    break;
                }

                case bloco_s_jogador:
                {
                    int xj = eixo_x + tamanho_bloco / 2;
                    int yj = eixo_y + tamanho_bloco / 2;

                    if(pos_spawn_jogador.x == 0)
                    {
                        pos_spawn_jogador.x = xj;
                        pos_spawn_jogador.y = yj;
                    }

                    break;
                }

                case bloco_s_fantasmas:
                {
                    int xf = eixo_x + tamanho_bloco / 2;
                    int yf = eixo_y + tamanho_bloco / 2;

                    if(pos_spawn_fantasmas.x == 0)
                    {
                        pos_spawn_fantasmas.x = xf;
                        pos_spawn_fantasmas.y = yf;
                    }

                    break;
                }

                default:
                    break;
            }

            eixo_x += tamanho_bloco;
        }

        eixo_y += tamanho_bloco;
        eixo_x = x_mapa;
    }


    // Coloca as entidades no spawn apenas na primeira vez
    if(!entidades_inicializadas)
    {
        for(int i = 0; i < quantidade_entidades; ++i)
        {
            if(entidades_para_render[i].tipo == entidade_fantasma)
            {
                entidades_para_render[i].pos = pos_spawn_fantasmas;
            }
            else if(entidades_para_render[i].tipo == entidade_pacman)
            {
                entidades_para_render[i].pos = pos_spawn_jogador;
            }
        }

        entidades_inicializadas = 1;
    }


    // Desenha as entidades
    for(int i = 0; i < quantidade_entidades; ++i)
    {
        struct entidade ent = entidades_para_render[i];

        switch(ent.tipo)
        {
            case entidade_fantasma:
                desenhar_fantasma(ent);
                break;

            case entidade_pacman:
                desenhar_pacman(ent);
                break;

            default:
                break;
        }
    }
}


void desenhar_display()
{
    int display_x = distancia_mapa / 2;
    int display_y = 60;

    DrawRectangle(
        display_x,
        display_y,
        tamanho_display,
        tamanho_display - 10,
        mapaFundo
    );

    Vector2 i, f;

    i.x = display_x;
    i.y = display_y;
    f.x = display_x + tamanho_display;
    f.y = display_y;
    DrawLineEx(i, f, 5, corBordas);

    i.x = display_x;
    i.y = display_y + tamanho_display - 10;
    f.x = display_x + tamanho_display;
    f.y = display_y + tamanho_display - 10;
    DrawLineEx(i, f, 5, corBordas);

    i.x = display_x;
    i.y = display_y;
    f.x = display_x;
    f.y = display_y + tamanho_display - 10;
    DrawLineEx(i, f, 5, corBordas);

    i.x = display_x + tamanho_display;
    i.y = display_y;
    f.x = display_x + tamanho_display;
    f.y = display_y + tamanho_display - 10;
    DrawLineEx(i, f, 5, corBordas);


    DrawText(
        "Info Jogador ",
        display_x + 10,
        display_y + 15,
        15,
        corBordas
    );

    DrawText(
        "Pontos restantes: ",
        display_x + 10,
        display_y + 40,
        15,
        WHITE
    );

    DrawText(
        "Vidas: ",
        display_x + 10,
        display_y + 70,
        15,
        WHITE
    );


    i.x = display_x + 10;
    i.y = display_y + 105;
    f.x = display_x + tamanho_display - 10;
    f.y = display_y + 105;

    DrawLineEx(i, f, 5, corBordas);


    DrawText(
        "Info Mapa ",
        display_x + 10,
        display_y + 120,
        15,
        corBordas
    );

    DrawText(
        "Nome do Mapa: ",
        display_x + 10,
        display_y + 145,
        15,
        WHITE
    );

    DrawText(
        "Total de Pontos: ",
        display_x + 10,
        display_y + 175,
        15,
        WHITE
    );
}


void iniciar_grafico()
{
    InitWindow(
        largura_tela,
        altura_tela,
        "PacMan - Trabalho Construcao de Algoritmos"
    );

    SetTargetFPS(FPS);

    while(!WindowShouldClose())
    {
        // Animacao dos pontos
        multi_alpha += (1.0 / FPS) * sentido_alpha;

        if(multi_alpha < 0.375f)
        {
            multi_alpha = 0.375f;
            sentido_alpha = 1;
        }

        if(multi_alpha > 1.0f)
        {
            multi_alpha = 1.0f;
            sentido_alpha = -1;
        }


        if(!mapa_escolhido)
        {
            break;
        }


        // So movimenta as entidades depois que receberam o spawn
        if(entidades_inicializadas)
        {
            atualizar_pacman();
            atualizar_fantasmas();
        }


        BeginDrawing();

            ClearBackground(corFundo);

            DrawText(
                "Pacman",
                20,
                10,
                40,
                WHITE
            );

            DrawText(
                "Construcao de Algoritmos",
                20 + MeasureText("Pacman", 40) + 10,
                30,
                15,
                WHITE
            );

            DrawText(
                "Erick Alves e Kauã Soares",
                20,
                altura_tela - 30,
                15,
                WHITE
            );

            desenhar_mapa();
            desenhar_display();

        EndDrawing();
    }

    CloseWindow();
}
