#include "grafico_jogo.h"
#include "fantasmas.h"
#include "pacman.h"

// Cores do jogo
const Color mapaFundo = {0, 0, 0, 255};
const Color corFundo1 = {3, 15, 79, 255};
const Color corFundo2 = {2, 24, 184, 255};
const Color corBordas = {6, 35, 212, 255};
const Color corDisplay = {12, 40, 250, 255};

// Variaveis para a animacao dos pontos
float multi_alpha = 1.0f;
int sentido_alpha = -1;

// Posicoes de spawn na tela
Vector2 pos_spawn_fantasmas = {0, 0};
Vector2 pos_spawn_jogador = {0, 0};

// Indica se as entidades ja foram posicionadas
int entidades_inicializadas = 0;

// Guarda o total original de pontos do mapa
int total_pontos_inicial = 0;


void desenhar_parede(int i, int j, int x, int y)
{
    Vector2 inicio_borda;
    Vector2 fim_borda;

    // Desenha as bordas externas do mapa
    if(j == 0 || j == tamanho_grid - 1)
    {
        int reajuste_x = (j != 0) * tamanho_bloco;

        inicio_borda.x = x + reajuste_x;
        inicio_borda.y = y;

        fim_borda.x = x + reajuste_x;
        fim_borda.y = y + tamanho_bloco;

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

        inicio_borda.x = x;
        inicio_borda.y = y + reajuste_y;

        fim_borda.x = x + tamanho_bloco;
        fim_borda.y = y + reajuste_y;

        DrawLineEx(
            inicio_borda,
            fim_borda,
            espessura_parede,
            corBordas
        );
    }

    // Desenha as bordas de uma parede quando
    // o bloco vizinho nao for outra parede
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

            if(vizinho_i >= 0 &&
               vizinho_i < tamanho_grid &&
               vizinho_j >= 0 &&
               vizinho_j < tamanho_grid)
            {
                enum tipo_bloco tipo_vizinho =
                    mapa_info.mapa[vizinho_i][vizinho_j].tipo;

                if(tipo_vizinho == bloco_parede)
                {
                    continue;
                }

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

    int tamanho = (int)(tamanho_bloco / 1.5f);

    DrawRectangle(
        (int)ent.pos.x - tamanho / 2,
        (int)ent.pos.y - tamanho / 2,
        tamanho,
        tamanho,
        ent.cor
    );
}


void desenhar_pacman(struct entidade ent)
{
    if(ent.tipo != entidade_pacman)
    {
        return;
    }

    int raio = (int)(tamanho_bloco / 2.5f);

    DrawCircle(
        (int)ent.pos.x,
        (int)ent.pos.y,
        raio,
        ent.cor
    );
}


void desenhar_mapa()
{
    int x_mapa =
        (largura_tela - tamanho_mapa) - distancia_mapa;

    int y_mapa =
        (altura_tela - tamanho_mapa) / 2;

    DrawRectangle(
        x_mapa,
        y_mapa,
        tamanho_mapa,
        tamanho_mapa,
        mapaFundo
    );

    int eixo_x = x_mapa;
    int eixo_y = y_mapa;

    for(int i = 0; i < tamanho_grid; ++i)
    {
        for(int j = 0; j < tamanho_grid; ++j)
        {
            switch(mapa_info.mapa[i][j].tipo)
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
                {
                    Color cor_ponto =
                    {
                        255,
                        255,
                        255,
                        (unsigned char)(255 * multi_alpha)
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
                    int xj =
                        eixo_x + tamanho_bloco / 2;

                    int yj =
                        eixo_y + tamanho_bloco / 2;

                    if(pos_spawn_jogador.x == 0)
                    {
                        pos_spawn_jogador.x = xj;
                        pos_spawn_jogador.y = yj;
                    }

                    break;
                }

                case bloco_s_fantasmas:
                {
                    int xf =
                        eixo_x + tamanho_bloco / 2;

                    int yf =
                        eixo_y + tamanho_bloco / 2;

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

    // Posiciona as entidades somente uma vez
    if(!entidades_inicializadas)
    {
        for(int i = 0; i < quantidade_entidades; ++i)
        {
            if(entidades_para_render[i].tipo == entidade_fantasma)
            {
                entidades_para_render[i].pos =
                    pos_spawn_fantasmas;
            }
            else if(entidades_para_render[i].tipo == entidade_pacman)
            {
                entidades_para_render[i].pos =
                    pos_spawn_jogador;
            }
        }

        total_pontos_inicial = mapa_info.total_pontos;
        entidades_inicializadas = 1;
    }
}


void desenhar_entidades()
{
    for(int i = 0; i < quantidade_entidades; ++i)
    {
        struct entidade ent =
            entidades_para_render[i];

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
    int display_x = distancia_mapa;
    int display_y = 55;

    int tamanho_x = tamanho_display;
    int tamanho_y = tamanho_display;

    int reajuste_x = 10;
    int reajuste_y = 15;

    DrawRectangle(
        display_x,
        display_y,
        tamanho_x,
        tamanho_y,
        mapaFundo
    );

    Vector2 inicio;
    Vector2 fim;

    inicio.x = display_x;
    inicio.y = display_y;

    fim.x = display_x + tamanho_x;
    fim.y = display_y;

    DrawLineEx(
        inicio,
        fim,
        5,
        corDisplay
    );

    inicio.x = display_x;
    inicio.y = display_y + tamanho_y;

    fim.x = display_x + tamanho_x;
    fim.y = display_y + tamanho_y;

    DrawLineEx(
        inicio,
        fim,
        5,
        corDisplay
    );

    inicio.x = display_x;
    inicio.y = display_y;

    fim.x = display_x;
    fim.y = display_y + tamanho_y;

    DrawLineEx(
        inicio,
        fim,
        5,
        corDisplay
    );

    inicio.x = display_x + tamanho_x;
    inicio.y = display_y;

    fim.x = display_x + tamanho_x;
    fim.y = display_y + tamanho_y;

    DrawLineEx(
        inicio,
        fim,
        5,
        corDisplay
    );

    DrawText(
        "Info Jogador",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        corDisplay
    );

    reajuste_y += 25;

    DrawText(
        "Pontos restantes:",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        WHITE
    );

    DrawText(
        TextFormat("%d", mapa_info.total_pontos),
        display_x + tamanho_x -
            MeasureText("888", 15) - 10,
        display_y + reajuste_y,
        15,
        WHITE
    );

    reajuste_y += 30;

    DrawText(
        "Vidas:",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        WHITE
    );

    DrawText(
        TextFormat("%d", vidas_pacman),
        display_x + tamanho_x -
            MeasureText("8", 15) - 10,
        display_y + reajuste_y,
        15,
        WHITE
    );

    reajuste_y += 35;

    DrawRectangle(
        display_x + reajuste_x,
        display_y + reajuste_y,
        tamanho_display - 20,
        3,
        corDisplay
    );

    reajuste_y += 15;

    DrawText(
        "Info Mapa",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        corDisplay
    );

    reajuste_y += 25;

    DrawText(
        "Indice do Mapa:",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        WHITE
    );

    DrawText(
        TextFormat("%d", mapa_info.indice_escolhido),
        display_x + tamanho_x -
            MeasureText("8", 15) - 10,
        display_y + reajuste_y,
        15,
        WHITE
    );

    reajuste_y += 30;

    DrawText(
        "Total de Pontos:",
        display_x + reajuste_x,
        display_y + reajuste_y,
        15,
        WHITE
    );

    DrawText(
        TextFormat("%d", total_pontos_inicial),
        display_x + tamanho_x -
            MeasureText("888", 15) - 10,
        display_y + reajuste_y,
        15,
        WHITE
    );
}


void desenhar_game_over()
{
    int largura_caixa = 300;
    int altura_caixa = 100;

    int x =
        (largura_tela - largura_caixa) / 2;

    int y =
        (altura_tela - altura_caixa) / 2;

    DrawRectangle(
        x,
        y,
        largura_caixa,
        altura_caixa,
        mapaFundo
    );

    DrawRectangleLines(
        x,
        y,
        largura_caixa,
        altura_caixa,
        corBordas
    );

    const char *texto = "GAME OVER";
    int tamanho_fonte = 35;

    int largura_texto =
        MeasureText(texto, tamanho_fonte);

    DrawText(
        texto,
        x + (largura_caixa - largura_texto) / 2,
        y + 30,
        tamanho_fonte,
        RED
    );
}


void desenhar_vitoria()
{
    int largura_caixa = 300;
    int altura_caixa = 100;

    int x =
        (largura_tela - largura_caixa) / 2;

    int y =
        (altura_tela - altura_caixa) / 2;

    DrawRectangle(
        x,
        y,
        largura_caixa,
        altura_caixa,
        mapaFundo
    );

    DrawRectangleLines(
        x,
        y,
        largura_caixa,
        altura_caixa,
        corBordas
    );

    const char *texto = "VOCE VENCEU!";
    int tamanho_fonte = 30;

    int largura_texto =
        MeasureText(texto, tamanho_fonte);

    DrawText(
        texto,
        x + (largura_caixa - largura_texto) / 2,
        y + 32,
        tamanho_fonte,
        YELLOW
    );
}


enum grafico_jogo iniciar_grafico()
{
    // Reseta informacoes graficas para uma nova partida
    pos_spawn_fantasmas = (Vector2){0, 0};
    pos_spawn_jogador = (Vector2){0, 0};

    entidades_inicializadas = 0;
    total_pontos_inicial = 0;

    multi_alpha = 1.0f;
    sentido_alpha = -1;

    InitWindow(
        largura_tela,
        altura_tela,
        "PacMan - Trabalho Construcao de Algoritmos"
    );

    SetTargetFPS(FPS);

    while(!WindowShouldClose())
    {
        // Animacao dos pontos
        multi_alpha +=
            (1.0f / FPS) * sentido_alpha;

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
            CloseWindow();
            return retorno_falha_mapa;
        }

        // Atualiza o jogo apenas enquanto
        // a partida estiver acontecendo
        if(entidades_inicializadas &&
           !game_over &&
           !jogador_venceu)
        {
            atualizar_pacman();
            atualizar_fantasmas();
            atualizar_colisao_pacman();
        }

        BeginDrawing();

            ClearBackground(corFundo1);

            DrawRectangleGradientH(
                0,
                0,
                largura_tela,
                altura_tela,
                corFundo1,
                corFundo2
            );

            DrawText(
                "Pacman",
                20,
                20,
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
                "Erick Alves e Kaua Soares",
                20,
                altura_tela - 30,
                15,
                WHITE
            );

            desenhar_mapa();
            desenhar_display();
            desenhar_entidades();

            if(game_over)
            {
                desenhar_game_over();
            }

            if(jogador_venceu)
            {
                desenhar_vitoria();
            }

        EndDrawing();

        if(game_over)
        {
            CloseWindow();
            return retorno_derrota;
        }

        if(jogador_venceu)
        {
            CloseWindow();
            return retorno_vitoria;
        }
    }

    CloseWindow();

    return retorno_sair;
}
