#include "core.h"
#include <ctype.h>

// ========== FUNÇÕES EXISTENTES (mantidas) ==========

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

// ========== NOVAS FUNÇÕES PARA CATÁLOGO ==========

CatalogoMusicas* carregar_catalogo_musicas() {
    FILE* arquivo = fopen(ARQUIVO_CATALOGO, "r");
    if (!arquivo) {
        printf("❌ Erro ao abrir catálogo de músicas!\n");
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
        // Remove newline
        linha[strcspn(linha, "\n")] = 0;
        
        // Pula linhas vazias
        if (strlen(linha) == 0) continue;
        
        // Expande array se necessário
        if (catalogo->qtdMusicas >= catalogo->capacidade) {
            int novaCap = catalogo->capacidade * 2;
            Musica* temp = realloc(catalogo->musicas, novaCap * sizeof(Musica));
            if (!temp) break;
            catalogo->musicas = temp;
            catalogo->capacidade = novaCap;
        }
        
        Musica* m = &catalogo->musicas[catalogo->qtdMusicas];
        
        // Parse da linha: título|artista|gênero|duração|info_extra|tipo_info
        char* token = strtok(linha, "|");
        if (token) strncpy(m->titulo, token, MAX-1);
        
        token = strtok(NULL, "|");
        if (token) strncpy(m->artista, token, MAX-1);
        
        token = strtok(NULL, "|");
        if (token) strncpy(m->genero, token, MAX-1);
        
        token = strtok(NULL, "|");
        if (token) m->duracao = atoi(token);
        
        token = strtok(NULL, "|");
        if (token) {
            int info_valor = atoi(token);
            token = strtok(NULL, "|");
            if (token) {
                m->tipoInfoExtra = atoi(token);
                if (m->tipoInfoExtra == 1) {
                    m->extra.anoLancamento = info_valor;
                } else {
                    m->extra.numeroAlbum = info_valor;
                }
            }
        }
        
        catalogo->qtdMusicas++;
    }
    
    fclose(arquivo);
    printf("📚 Catálogo carregado: %d músicas disponíveis\n", catalogo->qtdMusicas);
    return catalogo;
}

void liberar_catalogo(CatalogoMusicas* catalogo) {
    if (!catalogo) return;
    free(catalogo->musicas);
    free(catalogo);
}

//corrigindo erro de strcasestr (não existe na biblioteca padrão do C no windows)
int contem_ignore_case(const char* texto, const char* termo) {
    if (!texto || !termo) return 0;

    int len_texto = strlen(texto);
    int len_termo = strlen(termo);

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

int buscar_musica_catalogo(const CatalogoMusicas* catalogo, const char* termo, 
                          int tipo_busca, int* resultados, int max_resultados) {
    if (!catalogo || !termo || !resultados) return 0;
    
    int encontrados = 0;
    
    for (int i = 0; i < catalogo->qtdMusicas && encontrados < max_resultados; i++) {
        const Musica* m = &catalogo->musicas[i];
        const char* campo;
        
        switch (tipo_busca) {
            case 1: campo = m->titulo; break;    // Título
            case 2: campo = m->artista; break;   // Artista
            case 3: campo = m->genero; break;    // Gênero
            default: campo = m->titulo;
        }
        
        if (contem_ignore_case(campo, termo)) {
            resultados[encontrados] = i;
            encontrados++;
        }
    }
    
    return encontrados;
}

void exportar_playlist_txt(const Playlist* playlist, const char* nome_arquivo) {
    if (!playlist || playlist->qtdMusicas == 0) {
        printf("❌ Playlist vazia ou inválida!\n");
        return;
    }
    
    FILE* arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("❌ Erro ao criar arquivo de exportação!\n");
        return;
    }
    
    // Cabeçalho
    fprintf(arquivo, "========================================\n");
    fprintf(arquivo, "PLAYLIST: %s\n", playlist->nome);
    fprintf(arquivo, "Total de músicas: %d\n", playlist->qtdMusicas);
    
    int horas = playlist->duracaoTotal / 3600;
    int minutos = (playlist->duracaoTotal % 3600) / 60;
    int segundos = playlist->duracaoTotal % 60;
    
    if (horas > 0) {
        fprintf(arquivo, "Duração total: %02d:%02d:%02d\n", horas, minutos, segundos);
    } else {
        fprintf(arquivo, "Duração total: %02d:%02d\n", minutos, segundos);
    }
    
    fprintf(arquivo, "========================================\n\n");
    
    // Lista de músicas
    for (int i = 0; i < playlist->qtdMusicas; i++) {
        const Musica* m = &playlist->musicas[i];
        int min = m->duracao / 60;
        int seg = m->duracao % 60;
        
        fprintf(arquivo, "%d. %s\n", i + 1, m->titulo);
        fprintf(arquivo, "   Artista: %s\n", m->artista);
        fprintf(arquivo, "   Gênero: %s\n", m->genero);
        fprintf(arquivo, "   Duração: %02d:%02d\n", min, seg);
        
        if (m->tipoInfoExtra == 1) {
            fprintf(arquivo, "   Ano: %d\n", m->extra.anoLancamento);
        } else {
            fprintf(arquivo, "   Álbum: %d\n", m->extra.numeroAlbum);
        }
        
        fprintf(arquivo, "\n");
    }
    
    fclose(arquivo);
    printf("✅ Playlist exportada para: %s\n", nome_arquivo);
}

// ========== PERSISTÊNCIA ==========

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
