#ifndef PACMAN_H
#define PACMAN_H

#include "entidades.h"

// Quantidade atual de vidas do jogador
extern int vidas_pacman;

// Indica se a partida terminou
extern int game_over;

void atualizar_pacman(void);
void atualizar_colisao_pacman(void);

#endif
