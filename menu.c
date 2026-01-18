#include "operations.h"

/* ================= MENU ================= */

void mostrar_menu_principal() {
    printf("\n=== SISTEMA DE PLAYLISTS COM CATALOGO ===\n");
    printf("1. Criar Playlist\n");
    printf("2. Adicionar Musica do Catalogo\n");
    printf("3. Adicionar Musica Manualmente\n");
    printf("4. Remover Musica\n");
    printf("5. Listar Playlists\n");
    printf("6. Buscar no Catalogo\n");
    printf("7. Ordenar Playlist\n");
    printf("8. Estatisticas\n");
    printf("9. Exportar Playlist para TXT\n");
    printf("10. Salvar e Sair\n");
    printf("0. Sair sem Salvar\n");
    printf("========================================\n");
}

/* ================= LOOP PRINCIPAL ================= */

void executar_menu_principal(SistemaPlaylists* sistema, CatalogoMusicas* catalogo) {
    int executando = 1;

    while (executando) {
        mostrar_menu_principal();
        int opcao = obter_opcao_menu(0, 10);

        switch (opcao) {
            case OP_CRIAR_PLAYLIST:
                executar_opcao_criar_playlist(sistema);
                break;

            case OP_ADICIONAR_MUSICA_CATALOGO:
                executar_opcao_adicionar_musica_catalogo(sistema, catalogo);
                break;

            case OP_ADICIONAR_MUSICA_MANUAL:
                executar_opcao_adicionar_musica_manual(sistema);
                break;

            case OP_REMOVER_MUSICA:
                executar_opcao_remover_musica(sistema);
                break;

            case OP_LISTAR_PLAYLISTS:
                executar_opcao_listar_playlists(sistema);
                break;

            case OP_BUSCAR_CATALOGO:
                executar_opcao_buscar_catalogo(catalogo);
                break;

            case OP_ORDENAR_PLAYLIST:
                executar_opcao_ordenar_playlist(sistema);
                break;

            case OP_ESTATISTICAS:
                executar_opcao_estatisticas(sistema);
                break;

            case OP_EXPORTAR_PLAYLIST:
                executar_opcao_exportar_playlist(sistema);
                break;

            case OP_SALVAR_SAIR:
                salvar_sistema_arquivo(sistema);
                printf("Dados salvos. Saindo...\n");
                executando = 0;
                break;

            case OP_SAIR_SEM_SALVAR:
                printf("Saindo sem salvar...\n");
                executando = 0;
                break;

            default:
                printf("Opcao invalida! Tente novamente.\n");
                pausar_tela();
                break;
        }
    }
}
