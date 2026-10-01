#include "raylib.h"
#include "geranciador_de_mapa.h"

#define largura_tela 750
#define altura_tela (int)(largura_tela - (largura_tela*0.25))
#define distancia_mapa 40
#define FPS 30

#define espessura_parede 5
#define tamanho_mapa 450
#define tamanho_bloco (tamanho_mapa/tamanho_grid)

void iniciar_grafico(void); // Roda o gráfico do jogo