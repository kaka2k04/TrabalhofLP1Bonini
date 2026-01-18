#include "include/core.h"
#include <ctype.h>

// função interna - apenas ignora maiúsculas/minúsculas na busca
static int contem_ignore_case(const char* texto, const char* termo) {
    if (!texto || !termo) return 0;

    int len_texto = (int)strlen(texto);
    int len_termo = (int)strlen(termo);

    if (len_termo > len_texto) return 0;

    for (int i = 0; i <= len_texto - len_termo; i++) {
        int j = 0;
        while (j < len_termo &&
               tolower((unsigned char)texto[i + j]) ==
               tolower((unsigned char)termo[j])) {
            j++;
        }
        if (j == len_termo) return 1;
    }
    return 0;
}

CatalogoMusicas* carregar_catalogo_musicas() {
    FILE* arquivo = fopen(ARQUIVO_CATALOGO, "r");
    if (!arquivo) {
        printf("Ocorreu um erro ao abrir o catalogo de músicas!\n");
        return NULL;
    }

    CatalogoMusicas* catalogo = malloc(sizeof(CatalogoMusicas));
    if (!catalogo) {
        fclose(arquivo);
        return NULL;
    }

    catalogo->musicas = malloc(CAPACIDADE_INICIAL * sizeof(Musica));
    catalogo->qtdMusicas = 0;
    catalogo->capacidade = CAPACIDADE_INICIAL;

    char linha[500];
    while (fgets(linha, sizeof(linha), arquivo)) {
        linha[strcspn(linha, "\n")] = 0;
        if (strlen(linha) == 0) continue;

        if (catalogo->qtdMusicas >= catalogo->capacidade) {
            int novaCap = catalogo->capacidade * 2;
            Musica* temp = realloc(catalogo->musicas, novaCap * sizeof(Musica));
            if (!temp) break;
            catalogo->musicas = temp;
            catalogo->capacidade = novaCap;
        }

        Musica* m = &catalogo->musicas[catalogo->qtdMusicas];

        char* token = strtok(linha, "|");
        if (token) { strncpy(m->titulo, token, MAX-1); m->titulo[MAX-1] = '\0'; }

        token = strtok(NULL, "|");
        if (token) { strncpy(m->artista, token, MAX-1); m->artista[MAX-1] = '\0'; }

        token = strtok(NULL, "|");
        if (token) { strncpy(m->genero, token, MAX-1); m->genero[MAX-1] = '\0'; }

        token = strtok(NULL, "|");
        if (token) m->duracao = atoi(token);

        token = strtok(NULL, "|");
        if (token) {
            int info_valor = atoi(token);
            token = strtok(NULL, "|");
            if (token) {
                m->tipoInfoExtra = atoi(token);
                if (m->tipoInfoExtra == 1) m->extra.anoLancamento = info_valor;
                else m->extra.numeroAlbum = info_valor;
            }
        }

        catalogo->qtdMusicas++;
    }

    fclose(arquivo);
    printf("Catalogo carregado: %d musicas disponiveis\n", catalogo->qtdMusicas);
    return catalogo;
}

void liberar_catalogo(CatalogoMusicas* catalogo) {
    if (!catalogo) return;
    free(catalogo->musicas);
    free(catalogo);
}

int buscar_musica_catalogo(const CatalogoMusicas* catalogo, const char* termo,
                          int tipo_busca, int* resultados, int max_resultados) {
    if (!catalogo || !termo || !resultados) return 0;

    int encontrados = 0;

    for (int i = 0; i < catalogo->qtdMusicas && encontrados < max_resultados; i++) {
        const Musica* m = &catalogo->musicas[i];
        const char* campo;

        switch (tipo_busca) {
            case 1: campo = m->titulo; break;
            case 2: campo = m->artista; break;
            case 3: campo = m->genero; break;
            default: campo = m->titulo;
        }

        if (contem_ignore_case(campo, termo)) {
            resultados[encontrados++] = i;
        }
    }

    return encontrados;
}

void exportar_playlist_txt(const Playlist* playlist, const char* nome_arquivo) {
    if (!playlist || playlist->qtdMusicas == 0) {
        printf("Playlist vazia ou nao existe!\n");
        return;
    }

    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Ocorreu um erro ao criar arquivo de exportacao!\n");
        return;
    }

    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, "PLAYLIST: %s\n", playlist->nome);
    fprintf(arquivo, "Total de musicas: %d\n", playlist->qtdMusicas);

    int horas = playlist->duracaoTotal / 3600;
    int minutos = (playlist->duracaoTotal % 3600) / 60;
    int segundos = playlist->duracaoTotal % 60;

    if (horas > 0) fprintf(arquivo, "Duração total: %02d:%02d:%02d\n", horas, minutos, segundos);
    else fprintf(arquivo, "Duracao total: %02d:%02d\n", minutos, segundos);

    fprintf(arquivo, "========================================\n\n");

    for (int i = 0; i < playlist->qtdMusicas; i++) {
        const Musica* m = &playlist->musicas[i];
        int min = m->duracao / 60;
        int seg = m->duracao % 60;

        fprintf(arquivo, "%d. %s\n", i + 1, m->titulo);
        fprintf(arquivo, "   Artista: %s\n", m->artista);
        fprintf(arquivo, "   Genero: %s\n", m->genero);
        fprintf(arquivo, "   Duracao: %02d:%02d\n", min, seg);

        if (m->tipoInfoExtra == 1) fprintf(arquivo, "   Ano: %d\n", m->extra.anoLancamento);
        else fprintf(arquivo, "   Album: %d\n", m->extra.numeroAlbum);

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Playlist exportada para: %s\n", nome_arquivo);
}
