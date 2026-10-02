/*
Gerenciador de Mapa
------------------------------------------
Módulo responsavel pelo gerenciamento do mapa, gerindo o arquivo mapas.txt, salvando pontos de interesse,
escolha do mapa, etc...
*/
#define caminho "mapas.txt"
#define tamanho_grid 25

extern int mapa_escolhido;
enum tipo_bloco
{
    area_vazia,
    bloco_area_com_pontos,
    bloco_parede,
    bloco_s_jogador,
    bloco_s_fantasmas
};

struct bloco
{
    enum tipo_bloco tipo;
};

struct Mapa
{
    struct bloco mapa[tamanho_grid][tamanho_grid];
    int spawn_jogador[2];
    int spawn_fantasmas[2];
    int total_pontos;
    int indice_escolhido;
};
extern struct Mapa mapa_info;

int iniciar_data_mapas(void);
int escolher_mapa(void);
int quantidade_de_mapas(void);
int fechar_data_mapas(void);