#include "core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SistemaPlaylists* criar(){
    SistemaPlaylists* sistema;
    sistema=(SistemaPlaylists*)malloc(sizeof(SistemaPlaylists));

    if(sistema!=NULL){
        (*sistema).playlists=NULL;
        (*sistema).qtdPlaylists=0;
    }

    return sistema;
}

void liberar(SistemaPlaylists* sistema){
    if(sistema==NULL){
        return;
    }

    for(int i=0; i<(*sistema).qtdPlaylists; i++){
        free((*sistema).playlists[i].musicas);
    }

    free((*sistema).playlists);

    free(sistema);
}