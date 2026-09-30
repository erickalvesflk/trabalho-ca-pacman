/*
Gerenciador de Mapa
------------------------------------------
Módulo responsavel pelo gerenciamento do mapa, gerindo o arquivo mapas.txt, salvando pontos de interesse,
escolha do mapa, etc...
*/
extern struct Mapa mapa_obj;

int iniciar_data_mapas(void);
int escolher_mapa(void);
int quantidade_de_mapas(void);
int fechar_data_mapas(void);