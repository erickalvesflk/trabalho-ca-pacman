#include "raylib.h"
#include "geranciador_de_mapa.h"

#define largura_tela 750*1.5
#define altura_tela (int)(largura_tela - (largura_tela*0.25))
#define FPS 30

#define espessura_parede 5
#define tamanho_mapa 500
#define tamanho_bloco (tamanho_mapa/tamanho_grid)

void iniciar_grafico(void); // Roda o gráfico do jogo