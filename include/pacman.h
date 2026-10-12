#ifndef PACMAN_H
#define PACMAN_H

#include "entidades.h"

// Quantidade atual de vidas do jogador
extern int vidas_pacman;

// Indica se a partida terminou em derrota
extern int game_over;

// Indica se o jogador venceu
extern int jogador_venceu;

void atualizar_pacman(void);
void atualizar_colisao_pacman(void);

#endif
