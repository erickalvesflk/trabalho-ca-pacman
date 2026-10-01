#include "raylib.h"
enum direcao
{
    cima,
    baixo,
    esqueda,
    direita
};

struct fantasma 
{
    Vector2 pos;
    Color cor;
};

struct pacman
{
    Vector2 pos;
    Color cor;
};