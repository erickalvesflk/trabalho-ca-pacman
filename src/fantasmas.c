#include "fantasmas.h"

void atualizar_fantasmas(void)
{
    for(int i = 0; i < quantidade_entidades; i++)
    {
        if(entidades_para_render[i].tipo != entidade_fantasma)
        {
            continue;
        }

        // A movimentacao dos fantasmas vai entrar aqui
    }
}
