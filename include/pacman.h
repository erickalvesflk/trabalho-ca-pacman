#ifndef PACMAN_H
#define PACMAN_H

#include "entidades.h"
#define VIDAS 3
// Quantidade atual de vidas do jogador
extern int vidas_pacman;

// Indica se a partida terminou em derrota
extern int game_over;

// Indica se o jogador venceu
extern int jogador_venceu;

// Indica se o jogo esta parado
extern int congelado;

// Indica se é o primeio frame congelado
extern int primeiro_frame_congelado;

void atualizar_pacman(void);
void atualizar_colisao_pacman(void);
void inicializar_pacman(void);

#endif
