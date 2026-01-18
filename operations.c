#include "include/operations.h"
#include <ctype.h>
#include <string.h>

/* =========================================================
   Helpers locais (somente usados aqui no operations.c)
   ========================================================= */

static int comparar_titulos_ci(const char* a, const char* b) {
    /* comparação case-insensitive portátil */
    unsigned char ca, cb;
    while (*a && *b) {
        ca = (unsigned char)tolower((unsigned char)*a);
        cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb) return (int)ca - (int)cb;
        a++; b++;
    }
    return (int)tolower((unsigned char)*a) - (int)tolower((unsigned char)*b);
}

static int comparar_titulos(const Musica* a, const Musica* b) {
    return comparar_titulos_ci(a->titulo, b->titulo);
}

static int comparar_duracao(const Musica* a, const Musica* b) {
    return a->duracao - b->duracao;
}

static void bubble_sort(Musica* vetor, int n, int (*comparar)(const Musica*, const Musica*)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comparar(&vetor[j], &vetor[j + 1]) > 0) {
                Musica temp = vetor[j];
                vetor[j] = vetor[j + 1];
                vetor[j + 1] = temp;
            }
        }
    }
}

/* =========================================================
   FUNÇÕES EXECUTAR_OPCAO_* (1 a 9)
   ========================================================= */

void executar_opcao_criar_playlist(SistemaPlaylists* sistema) {
    char nome[MAX];
    printf("Nome da nova playlist: ");
    fgets(nome, MAX, stdin);
    nome[strcspn(nome, "\n")] = 0;

    if (strlen(nome) == 0) {
        printf("O nome nao pode estar vazio!\n");
        pausar_tela();
        return;
    }

    if (adicionar_playlist(sistema, nome)) {
        printf("Sua Playlist '%s' foi criada com sucesso!\n", nome);
    } else {
        printf("Erro ao criar playlist.\n");
    }

    pausar_tela();
}

void executar_opcao_adicionar_musica_catalogo(SistemaPlaylists* sistema, CatalogoMusicas* catalogo) {
    if (!catalogo || catalogo->qtdMusicas == 0) {
        printf("Catalogo nao esta disponivel ou esta vazio!\n");
        pausar_tela();
        return;
    }

    if (sistema->qtdPlaylists == 0) {
        printf("Crie uma playlist primeiro!\n");
        pausar_tela();
        return;
    }

    printf("Buscar musica no catalogo:\n");
    printf("1. Por titulo\n");
    printf("2. Por artista\n");
    printf("3. Por genero\n");
    printf("Opcao: ");

    int tipo_busca;
    if (scanf("%d", &tipo_busca) != 1 || tipo_busca < 1 || tipo_busca > 3) {
        printf("Opcao invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();

    char termo[MAX];
    const char* termos[] = {"titulo", "artista", "genero"};
    printf("Digite o %s: ", termos[tipo_busca - 1]);
    fgets(termo, MAX, stdin);
    termo[strcspn(termo, "\n")] = 0;

    if (strlen(termo) == 0) {
        printf("O termo de busca nao pode ser vazio!\n");
        pausar_tela();
        return;
    }

    int resultados[50];
    int encontrados = buscar_musica_catalogo(catalogo, termo, tipo_busca, resultados, 50);

    if (encontrados == 0) {
        printf("Nenhuma musica foi encontrada.\n");
        pausar_tela();
        return;
    }

    printf("\nMusicas encontradas (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        const Musica* m = &catalogo->musicas[resultados[i]];
        printf("%d. %s - %s (%s, %d segundos)\n",
               i + 1, m->titulo, m->artista, m->genero, m->duracao);
    }

    printf("\nSelecione a musica (1 a %d) ou 0 para cancelar: ", encontrados);
    int escolha;
    if (scanf("%d", &escolha) != 1 || escolha < 0 || escolha > encontrados) {
        printf("Escolha invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    if (escolha == 0) {
        printf("Operacao cancelada.\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    printf("\nSelecione a playlist para adicionar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }

    int playlist_idx;
    printf("Playlist numero: ");
    if (scanf("%d", &playlist_idx) != 1 ||
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("Playlist invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    int musica_idx = resultados[escolha - 1];
    const Musica* musica_selecionada = &catalogo->musicas[musica_idx];

    if (adicionar_musica_playlist(&sistema->playlists[playlist_idx - 1], musica_selecionada)) {
        printf("Musica '%s' adicionada a playlist '%s'!\n",
               musica_selecionada->titulo,
               sistema->playlists[playlist_idx - 1].nome);
    } else {
        printf("Erro ao adicionar musica.\n");
    }

    limpar_buffer();
    pausar_tela();
}

void executar_opcao_adicionar_musica_manual(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Crie uma playlist primeiro!\n");
        pausar_tela();
        return;
    }

    printf("Selecione a playlist:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }

    int playlist_idx;
    printf("Playlist numero: ");
    if (scanf("%d", &playlist_idx) != 1 ||
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("Playlist invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();

    Musica m;

    printf("Titulo: ");
    fgets(m.titulo, MAX, stdin);
    m.titulo[strcspn(m.titulo, "\n")] = 0;

    printf("Artista: ");
    fgets(m.artista, MAX, stdin);
    m.artista[strcspn(m.artista, "\n")] = 0;

    printf("Genero: ");
    fgets(m.genero, MAX, stdin);
    m.genero[strcspn(m.genero, "\n")] = 0;

    printf("Duracao (segundos): ");
    if (scanf("%d", &m.duracao) != 1 || m.duracao <= 0) {
        printf("Duracao invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    printf("Informacao extra:\n");
    printf("1. Ano de lancamento\n");
    printf("2. Numero do album\n");
    printf("Opcao: ");
    if (scanf("%d", &m.tipoInfoExtra) != 1 || (m.tipoInfoExtra != 1 && m.tipoInfoExtra != 2)) {
        printf("Opcao invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    if (m.tipoInfoExtra == 1) {
        printf("Ano: ");
        if (scanf("%d", &m.extra.anoLancamento) != 1) {
            printf("Ano invalido!\n");
            limpar_buffer();
            pausar_tela();
            return;
        }
    } else {
        printf("Numero do album: ");
        if (scanf("%d", &m.extra.numeroAlbum) != 1) {
            printf("Numero do album invalido!\n");
            limpar_buffer();
            pausar_tela();
            return;
        }
    }

    if (adicionar_musica_playlist(&sistema->playlists[playlist_idx - 1], &m)) {
        printf("Musica adicionada com sucesso!\n");
    } else {
        printf("Ocorreu um erro ao adicionar musica.\n");
    }

    limpar_buffer();
    pausar_tela();
}

void executar_opcao_remover_musica(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Nenhuma playlist disponivel!\n");
        pausar_tela();
        return;
    }

    printf("Selecione a playlist:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s (%d musica%s)\n",
               i + 1,
               sistema->playlists[i].nome,
               sistema->playlists[i].qtdMusicas,
               sistema->playlists[i].qtdMusicas != 1 ? "s" : "");
    }

    int playlist_idx;
    printf("Playlist numero: ");
    if (scanf("%d", &playlist_idx) != 1 ||
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("Playlist invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    Playlist* playlist = &sistema->playlists[playlist_idx - 1];

    if (playlist->qtdMusicas == 0) {
        printf("Esta playlist esta vazia!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    printf("\nMusicas na playlist '%s':\n", playlist->nome);
    for (int i = 0; i < playlist->qtdMusicas; i++) {
        printf("%d. %s - %s\n",
               i + 1,
               playlist->musicas[i].titulo,
               playlist->musicas[i].artista);
    }

    int musica_idx;
    printf("\nMusica numero a remover: ");
    if (scanf("%d", &musica_idx) != 1 ||
        musica_idx < 1 || musica_idx > playlist->qtdMusicas) {
        printf("Musica invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    printf("Remover '%s'? (s/n): ", playlist->musicas[musica_idx - 1].titulo);
    char confirmacao;
    limpar_buffer();
    scanf("%c", &confirmacao);

    if (tolower((unsigned char)confirmacao) == 's') {
        if (remover_musica_playlist(playlist, musica_idx - 1)) {
            printf("Musica removida com sucesso!\n");
        } else {
            printf("Erro ao remover musica.\n");
        }
    } else {
        printf("Operacao cancelada.\n");
    }

    limpar_buffer();
    pausar_tela();
}

void executar_opcao_listar_playlists(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Nenhuma playlist criada.\n");
        pausar_tela();
        return;
    }

    printf("PLAYLISTS DISPONIVEIS:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        Playlist* p = &sistema->playlists[i];

        int horas = p->duracaoTotal / 3600;
        int minutos = (p->duracaoTotal % 3600) / 60;
        int segundos = p->duracaoTotal % 60;

        printf("\n%s\n", p->nome);
        printf("  Musicas: %d\n", p->qtdMusicas);

        if (horas > 0) {
            printf("  Duracao total: %02d:%02d:%02d\n", horas, minutos, segundos);
        } else {
            printf("  Duracao total: %02d:%02d\n", minutos, segundos);
        }

        if (p->qtdMusicas > 0) {
            printf("  Lista de musicas:\n");
            for (int j = 0; j < p->qtdMusicas; j++) {
                int min_musica = p->musicas[j].duracao / 60;
                int seg_musica = p->musicas[j].duracao % 60;
                printf("   %d. %s - %s (%02d:%02d)\n",
                       j + 1,
                       p->musicas[j].titulo,
                       p->musicas[j].artista,
                       min_musica, seg_musica);
            }
        }
    }

    pausar_tela();
}

void executar_opcao_buscar_catalogo(CatalogoMusicas* catalogo) {
    if (!catalogo || catalogo->qtdMusicas == 0) {
        printf("Catalogo nao disponivel!\n");
        pausar_tela();
        return;
    }

    printf("Buscar no catalogo:\n");
    printf("1. Por titulo\n");
    printf("2. Por artista\n");
    printf("3. Por genero\n");
    printf("Opcao: ");

    int tipo_busca;
    if (scanf("%d", &tipo_busca) != 1 || tipo_busca < 1 || tipo_busca > 3) {
        printf("Opcao invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();

    char termo[MAX];
    const char* termos[] = {"titulo", "artista", "genero"};
    printf("Digite o %s: ", termos[tipo_busca - 1]);
    fgets(termo, MAX, stdin);
    termo[strcspn(termo, "\n")] = 0;

    if (strlen(termo) == 0) {
        printf("Termo de busca nao pode ser vazio!\n");
        pausar_tela();
        return;
    }

    int resultados[100];
    int encontrados = buscar_musica_catalogo(catalogo, termo, tipo_busca, resultados, 100);

    if (encontrados == 0) {
        printf("Nenhuma musica encontrada.\n");
    } else {
        printf("\nResultados da busca (%d encontrados):\n", encontrados);
        for (int i = 0; i < encontrados; i++) {
            const Musica* m = &catalogo->musicas[resultados[i]];
            int min = m->duracao / 60;
            int seg = m->duracao % 60;

            printf("%d. %s\n", i + 1, m->titulo);
            printf("   Artista: %s\n", m->artista);
            printf("   Genero: %s\n", m->genero);
            printf("   Duracao: %02d:%02d\n", min, seg);

            if (m->tipoInfoExtra == 1) {
                printf("   Ano: %d\n", m->extra.anoLancamento);
            } else {
                printf("   Album: %d\n", m->extra.numeroAlbum);
            }

            if (i < encontrados - 1) printf("\n");
        }
    }

    pausar_tela();
}

void executar_opcao_ordenar_playlist(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Nenhuma playlist disponivel!\n");
        pausar_tela();
        return;
    }

    printf("Selecione a playlist para ordenar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }

    int playlist_idx;
    printf("Playlist numero: ");
    if (scanf("%d", &playlist_idx) != 1 ||
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("Playlist invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    Playlist* playlist = &sistema->playlists[playlist_idx - 1];

    if (playlist->qtdMusicas < 2) {
        printf("A playlist precisa ter pelo menos 2 musicas!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    printf("Criterio de ordenacao:\n");
    printf("1. Titulo (A-Z)\n");
    printf("2. Duracao (menor para maior)\n");
    printf("Opcao: ");

    int criterio;
    if (scanf("%d", &criterio) != 1 || (criterio != 1 && criterio != 2)) {
        printf("Criterio invalido!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    if (criterio == 1) {
        bubble_sort(playlist->musicas, playlist->qtdMusicas, comparar_titulos);
        printf("Playlist ordenada por titulo!\n");
    } else {
        bubble_sort(playlist->musicas, playlist->qtdMusicas, comparar_duracao);
        printf("Playlist ordenada por duracao!\n");
    }

    limpar_buffer();
    pausar_tela();
}

void executar_opcao_estatisticas(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Nenhuma playlist criada.\n");
        pausar_tela();
        return;
    }

    printf("ESTATISTICAS DO SISTEMA:\n");

    int total_musicas = 0;
    int total_duracao = 0;
    Playlist* maior_playlist = &sistema->playlists[0];
    Playlist* menor_playlist = &sistema->playlists[0];

    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        Playlist* p = &sistema->playlists[i];

        total_musicas += p->qtdMusicas;
        total_duracao += p->duracaoTotal;

        if (p->qtdMusicas > maior_playlist->qtdMusicas) maior_playlist = p;
        if (p->qtdMusicas < menor_playlist->qtdMusicas) menor_playlist = p;
    }

    int horas_total = total_duracao / 3600;
    int minutos_total = (total_duracao % 3600) / 60;

    printf("Total de playlists: %d\n", sistema->qtdPlaylists);
    printf("Total de musicas: %d\n", total_musicas);

    if (horas_total > 0) {
        printf("Duracao total: %02d:%02d:%02d\n", horas_total, minutos_total, total_duracao % 60);
    } else {
        printf("Duracao total: %02d:%02d\n", minutos_total, total_duracao % 60);
    }

    if (total_musicas > 0) {
        float media_por_playlist = (float)total_musicas / sistema->qtdPlaylists;
        float media_duracao = (float)total_duracao / total_musicas;

        printf("Media de musicas por playlist: %.1f\n", media_por_playlist);
        printf("Duracao media por musica: %.0f segundos\n", media_duracao);
    }

    printf("\nPlaylist com mais musicas: %s (%d musica%s)\n",
           maior_playlist->nome, maior_playlist->qtdMusicas,
           maior_playlist->qtdMusicas != 1 ? "s" : "");

    printf("Playlist com menos musicas: %s (%d musica%s)\n",
           menor_playlist->nome, menor_playlist->qtdMusicas,
           menor_playlist->qtdMusicas != 1 ? "s" : "");

    pausar_tela();
}

void executar_opcao_exportar_playlist(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("Nenhuma playlist disponivel!\n");
        pausar_tela();
        return;
    }

    printf("Selecione a playlist para exportar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s (%d musica%s)\n",
               i + 1,
               sistema->playlists[i].nome,
               sistema->playlists[i].qtdMusicas,
               sistema->playlists[i].qtdMusicas != 1 ? "s" : "");
    }

    int playlist_idx;
    printf("Playlist numero: ");
    if (scanf("%d", &playlist_idx) != 1 ||
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("Playlist invalida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    char nome_arquivo[MAX];
    printf("Nome do arquivo para exportacao (ex: minha_playlist.txt): ");
    limpar_buffer();
    fgets(nome_arquivo, MAX, stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;

    if (strlen(nome_arquivo) == 0) {
        strcpy(nome_arquivo, ARQUIVO_PLAYLIST_EXPORT);
    }

    Playlist* playlist = &sistema->playlists[playlist_idx - 1];
    exportar_playlist_txt(playlist, nome_arquivo);

    pausar_tela();
}
