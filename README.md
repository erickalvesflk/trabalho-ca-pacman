# Trabalho Pacman - UERJ
  Trabalho onde Recriamos o jogo clássico PACMAN utilizando apenas a linguagem C
  e a biblioteca de criação de jogos RAYLIB. Em geral, Kauã Soares foi responável
  pelo núcleo do jogo (Sistemas dos fantasmas, pacman e pontos) e Erick Alves foi
  responsável pela parte gráfica (Criação e Gerenciamento de Mapas, Sprites e Gráfico)

  Professor: Eugênio Silva
  Participantes:
  - Erick Alves dos Santos
  - Kauã Soares dos Santos Rodrigues

# Fluxo do jogo
```
  ┌─────────────────────────────────────────────┐
  │ Configura a janela do jogo                  │
  └──┬──────────────────────────────────────────┘
     │                    
     ▼                    
  ┌─────────────────────────────────────────────┐
  │ Gerenciamento de mapa                       │◄──────────────┐
  │ Carrega o arquivo mapa.txt e escolhe um mapa│               │
  │ de forma aleatória                          │               │
  └────────────────┬────────────────────────────┘               │
                   │                                            │
                   ▼                                            │
  ┌─────────────────────────────────────────────┐               │
  │ Iniciação do gráfico                        │               │
  └────────────────┬────────────────────────────┘               │
                   │                                            │
                   ▼                                            │
  ┌─────────────────────────────────────────────┐               │
  │ Inicia o Pac-Man e as variáveis do jogo     │               │
  └────────────────┬────────────────────────────┘               │
                   │                                            │
     ┌─────────────▼───────────────────────┐                    │
     │ LOOP PRINCIPAL (30 vezes/segundo)   │◄─────────────┐     │
     └───────────────────┬─────────────────┘              │     │
         ┌ ─ ─ ─ ─ ─ ─ ─ ▼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┐       │     │
         │         PARTE DE PROCESSAMENTO         │       │     │
         └ ─ ─ ─ ─ ─ ─ ─ ┬ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┘       │     │
                         ▼                                │     │
         ┌────────────────────────────────────────┐       │     │
         │ Avança as animações do jogo            │       │     │
         └───────────────────┬────────────────────┘       │     │
                             ▼                            │     │
         ┌─────────────────────────────────────────┐      │     │
         │ Atualiza a posição do Pac-Man           │      │     │
         ├─────────────────────────────────────────┤      │     │
         │ Atualiza a posição dos fantasmas (BFS)  │      │     │
         ├─────────────────────────────────────────┤      │     │
         │ Atualiza a colisão do Pac-Man com o mapa│      │     │
         └───────────────────┬─────────────────────┘      │     │
         ┌ ─ ─ ─ ─ ─ ─ ─ ─ ─ ▼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┐       │     │
         │       PARTE DE DESENHO DO GRÁFICO      │       │     │
         └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┬ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┘       │     │
                             ▼                            │     │
         ┌────────────────────────────────────────┐       │     │
         │ Limpa a janela e desenha o fundo       │       │     │
         └───────────────────┬────────────────────┘       │     │
                             ▼                            │     │
         ┌────────────────────────────────────────┐       │     │
         │ Desenha o mapa, seguindo a estrutura   │       │     │
         │ informada pelo mapa.txt em uma matriz  │       │     │
         ├────────────────────────────────────────┤       │     │
         │ Desenha o display de informações       │       │     │
         ├────────────────────────────────────────┤       │     │
         │ Desenha o Pac-Man e os fantasmas       │       │     │
         └───────────────────┬────────────────────┘       │     │
         ┌ ─ ─ ─ ─ ─ ─ ─ ─ ─ ▼ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┐       │     │
         │          PARTE DE VERIFICAÇÃO          │       │     │
         └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┬ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┘       │     │
                             ▼                            │     │
         ┌────────────────────────────────────────┐       │     │
         │ Verifica se o jogador ganhou ou perdeu │       │     │
         └─────────┬────────────────────┬─────────┘       │     │
                   │                    │                 │     │
                 Falso               Verdadeiro           │     │
                   │                    │                 │     │
                   └────────────────────┼─────────────────┘     │
                                        │                       │
                                        ▼                       │
         ┌────────────────────────────────────────┐             │
         │ Caso o jogador queira repetir,         │             │
         │ resetamos o jogo                       │─────────────┘
         └────────────────────────────────────────┘
```
# Instalação Executável
Utilize a seção `realeases` do github para baixar apenas os arquivos necessários para executar o jogo. Atualmente, o zip contém:
```
pacman
├ /src/ - Contém os sprites do jogo
├ mapas.txt - Contém todas as configurações de mapas
└ pacman (executável) -  Executável do jogo
```
# Instalação para Edição
## Depêndecias
- Raylib
- Placa de video com OpenGl (Caso falhe, tente baixar o drive de video da placa de video)
## Configuração Para compliação
Para conseguir compilar o projeto, utilize o .vscode/tasks.json já configurado (Apenas para vscode). Nesse arquivo, altere `"command": "..."` pelo o seu caminho e
caso baixe o raylib de forma universal altere a localização de `"-I${workspaceFolder}/raylib",` para a localização do `librarylib.a` do seu raylib e `"-L${workspaceFolder}/raylib",` para a localização do `raylib.h`. **Resumo: Altere o .vscode/tasks.json para:**
```
"command": "<Endereço do executável do seu compilador C>"
"-I${workspaceFolder}/raylib", ────(Para)────> "-I<Endereço do librarylib.a do seu raylib>"
"-L${workspaceFolder}/raylib", ────(Para)────> "-L<Endereço do raylib.h>"
```
---
**Obrigado pela a atenção e Bom Jogo!**
---
