#include "include/core.h"

void salvar_sistema_arquivo(const SistemaPlaylists* sistema) {
    FILE* arquivo = fopen(ARQUIVO_DADOS, "wb");
    if (!arquivo) return;

    fwrite(&sistema->qtdPlaylists, sizeof(int), 1, arquivo);

    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        Playlist* p = &sistema->playlists[i];
        fwrite(p->nome, sizeof(char), MAX, arquivo);
        fwrite(&p->qtdMusicas, sizeof(int), 1, arquivo);
        fwrite(&p->duracaoTotal, sizeof(int), 1, arquivo);

        if (p->qtdMusicas > 0) {
            fwrite(p->musicas, sizeof(Musica), p->qtdMusicas, arquivo);
        }
    }

    fclose(arquivo);
}

void carregar_sistema_arquivo(SistemaPlaylists* sistema) {
    FILE* arquivo = fopen(ARQUIVO_DADOS, "rb");
    if (!arquivo) return;

    int qtdPlaylistsArq;
    if (fread(&qtdPlaylistsArq, sizeof(int), 1, arquivo) != 1) {
        fclose(arquivo);
        return;
    }

    for (int i = 0; i < qtdPlaylistsArq; i++) {
        char nome[MAX];
        int qtdMusicas, duracaoTotal;

        fread(nome, sizeof(char), MAX, arquivo);
        fread(&qtdMusicas, sizeof(int), 1, arquivo);
        fread(&duracaoTotal, sizeof(int), 1, arquivo);

        adicionar_playlist(sistema, nome);

        Playlist* p = &sistema->playlists[i];
        p->duracaoTotal = duracaoTotal;

        if (qtdMusicas > 0) {
            p->musicas = malloc(qtdMusicas * sizeof(Musica));
            fread(p->musicas, sizeof(Musica), qtdMusicas, arquivo);
            p->qtdMusicas = qtdMusicas;
            p->capacidade = qtdMusicas;
        }
    }

    fclose(arquivo);
}
