#include <stdio.h>
#include "include/core.h"
#include "include/operations.h"

int main(void) {
    // cria o sistema
    SistemaPlaylists* sistema = criar_sistema();
    if (!sistema) {
        printf("Falha ao alocar o sistema.\n");
        return 1;
    }

    // caso existe, carrega os dados salvos
    carregar_sistema_arquivo(sistema);

    // carrega o catálogo de músicas
    CatalogoMusicas* catalogo = carregar_catalogo_musicas();
    if (!catalogo) {
        printf("Catalogo não carregado. Voce ainda pode criar playlists e adicionar musicas manualmente.\n");
    }

    // executa o menu principal
    executar_menu_principal(sistema, catalogo);

    // libera recursos
    liberar_catalogo(catalogo);
    liberar_sistema(sistema);

    printf("Programa finalizado.\n");
    return 0;
}
