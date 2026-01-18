#include "core.h"

SistemaPlaylists* criar_sistema() {
    SistemaPlaylists* sistema = malloc(sizeof(SistemaPlaylists));
    if (sistema) {
        sistema->playlists = NULL;
        sistema->qtdPlaylists = 0;
        sistema->capacidade = 0;
    }
    return sistema;
}

void liberar_sistema(SistemaPlaylists* sistema) {
    if (!sistema) return;

    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        free(sistema->playlists[i].musicas);
    }
    free(sistema->playlists);
    free(sistema);
}

int adicionar_playlist(SistemaPlaylists* sistema, const char* nome) {
    if (sistema->qtdPlaylists >= sistema->capacidade) {
        int novaCap = sistema->capacidade ? sistema->capacidade * 2 : CAPACIDADE_INICIAL;
        Playlist* temp = realloc(sistema->playlists, novaCap * sizeof(Playlist));
        if (!temp) return 0;
        sistema->playlists = temp;
        sistema->capacidade = novaCap;
    }

    Playlist* nova = &sistema->playlists[sistema->qtdPlaylists];
    strncpy(nova->nome, nome, MAX-1);
    nova->nome[MAX-1] = '\0';
    nova->musicas = NULL;
    nova->qtdMusicas = 0;
    nova->capacidade = 0;
    nova->duracaoTotal = 0;

    sistema->qtdPlaylists++;
    return 1;
}

int encontrar_playlist_index(const SistemaPlaylists* sistema, const char* nome) {
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        if (strcmp(sistema->playlists[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

int adicionar_musica_playlist(Playlist* playlist, const Musica* musica) {
    if (playlist->qtdMusicas >= playlist->capacidade) {
        int novaCap = playlist->capacidade ? playlist->capacidade * 2 : CAPACIDADE_INICIAL;
        Musica* temp = realloc(playlist->musicas, novaCap * sizeof(Musica));
        if (!temp) return 0;
        playlist->musicas = temp;
        playlist->capacidade = novaCap;
    }

    playlist->musicas[playlist->qtdMusicas] = *musica;
    playlist->qtdMusicas++;
    playlist->duracaoTotal += musica->duracao;
    return 1;
}

int remover_musica_playlist(Playlist* playlist, int indice) {
    if (indice < 0 || indice >= playlist->qtdMusicas) return 0;

    int duracao_removida = playlist->musicas[indice].duracao;

    for (int i = indice; i < playlist->qtdMusicas - 1; i++) {
        playlist->musicas[i] = playlist->musicas[i + 1];
    }

    playlist->qtdMusicas--;
    playlist->duracaoTotal -= duracao_removida;
    return 1;
}

void atualizar_duracao_total(Playlist* playlist) {
    playlist->duracaoTotal = 0;
    for (int i = 0; i < playlist->qtdMusicas; i++) {
        playlist->duracaoTotal += playlist->musicas[i].duracao;
    }
}
