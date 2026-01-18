#include "include/operations.h"
#include <string.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar_tela() {
    printf("\nAperte ENTER para continuar...");
    getchar();
}

int obter_opcao_menu(int min, int max) {
    int opcao;
    printf("Opção: ");
    if (scanf("%d", &opcao) != 1) {
        limpar_buffer();
        return -1;
    }
    limpar_buffer();
    return (opcao >= min && opcao <= max) ? opcao : -1;
}
