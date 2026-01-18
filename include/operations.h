#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "core.h"

#define OP_CRIAR_PLAYLIST 1
#define OP_ADICIONAR_MUSICA_CATALOGO 2
#define OP_ADICIONAR_MUSICA_MANUAL 3
#define OP_REMOVER_MUSICA 4
#define OP_LISTAR_PLAYLISTS 5
#define OP_BUSCAR_CATALOGO 6
#define OP_ORDENAR_PLAYLIST 7
#define OP_ESTATISTICAS 8
#define OP_EXPORTAR_PLAYLIST 9
#define OP_SALVAR_SAIR 10
#define OP_SAIR_SEM_SALVAR 0

void mostrar_menu_principal();
void executar_menu_principal(SistemaPlaylists* sistema, CatalogoMusicas* catalogo);

void executar_opcao_criar_playlist(SistemaPlaylists* sistema);
void executar_opcao_adicionar_musica_catalogo(SistemaPlaylists* sistema, CatalogoMusicas* catalogo);
void executar_opcao_adicionar_musica_manual(SistemaPlaylists* sistema);
void executar_opcao_remover_musica(SistemaPlaylists* sistema);
void executar_opcao_listar_playlists(SistemaPlaylists* sistema);
void executar_opcao_buscar_catalogo(CatalogoMusicas* catalogo);
void executar_opcao_ordenar_playlist(SistemaPlaylists* sistema);
void executar_opcao_estatisticas(SistemaPlaylists* sistema);
void executar_opcao_exportar_playlist(SistemaPlaylists* sistema);

int obter_opcao_menu(int min, int max);
void limpar_buffer();
void pausar_tela();

#endif
