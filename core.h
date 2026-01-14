#ifndef CORE_H
#define CORE_H

#define MAX 100

typedef union{
    int anoLancamento;
    int numeroAlbum;
}InfoExtra;

typedef struct{
    char titulo[MAX];
    char artista[MAX];
    int duracao;
    char genero[MAX];
    InfoExtra extra;
    int tipoInfoExtra;
}Musica;

typedef struct{
    char nome[MAX];
    Musica *musicas;
    int qtdMusicas;
    int duracaoTotal;
}Playlist;

typedef struct{
    Playlist *playlists;
    int qtdPlaylists;
}SistemaPlaylists;

SistemaPlaylists* criar();
void liberar(SistemaPlaylists* sistema);

#endif