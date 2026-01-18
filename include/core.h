#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define ARQUIVO_DADOS "playlists.dat"
#define ARQUIVO_CATALOGO "catalogo_musicas.txt"
#define ARQUIVO_PLAYLIST_EXPORT "playlist_exportada.txt"
#define CAPACIDADE_INICIAL 5

typedef union {
    int anoLancamento;
    int numeroAlbum;
} InfoExtra;

typedef struct {
    char titulo[MAX];
    char artista[MAX];
    int duracao;
    char genero[MAX];
    InfoExtra extra;
    int tipoInfoExtra;
} Musica;

typedef struct {
    Musica* musicas;
    int qtdMusicas;
    int capacidade;
} CatalogoMusicas;

typedef struct {
    char nome[MAX];
    Musica *musicas;
    int qtdMusicas;
    int capacidade;
    int duracaoTotal;
} Playlist;

typedef struct {
    Playlist *playlists;
    int qtdPlaylists;
    int capacidade;
} SistemaPlaylists;

// Sistema
SistemaPlaylists* criar_sistema();
void liberar_sistema(SistemaPlaylists* sistema);

// Playlist
int adicionar_playlist(SistemaPlaylists* sistema, const char* nome);
int encontrar_playlist_index(const SistemaPlaylists* sistema, const char* nome);
int adicionar_musica_playlist(Playlist* playlist, const Musica* musica);
int remover_musica_playlist(Playlist* playlist, int indice);
void atualizar_duracao_total(Playlist* playlist);

// Catalogo
CatalogoMusicas* carregar_catalogo_musicas();
void liberar_catalogo(CatalogoMusicas* catalogo);
int buscar_musica_catalogo(const CatalogoMusicas* catalogo, const char* termo, 
                          int tipo_busca, int* resultados, int max_resultados);
void exportar_playlist_txt(const Playlist* playlist, const char* nome_arquivo);

// Persistencia
void salvar_sistema_arquivo(const SistemaPlaylists* sistema);
void carregar_sistema_arquivo(SistemaPlaylists* sistema);

#endif
