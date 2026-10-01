#include "raylib.h"
#define quantidade_entidades 6

enum direcao
{
    cima,
    baixo,
    esqueda,
    direita
};
enum tipo_entidade
{
    entidade_fantasma,
    entidade_pacman,
    entidade_vazia
};
struct entidade
{
    enum tipo_entidade tipo;
    Vector2 pos;
    Color cor;
};

extern struct entidade entidades_para_render[quantidade_entidades];
void gerar_entidades(void);