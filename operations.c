#include "operations.h"
#include <ctype.h>
#include <string.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar_tela() {
    printf("\nPressione ENTER para continuar...");
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

void mostrar_menu_principal() {
    printf("\n=== SISTEMA DE PLAYLISTS COM CATÁLOGO ===\n");
    printf("1. Criar Playlist\n");
    printf("2. Adicionar Música do Catálogo\n");
    printf("3. Adicionar Música Manualmente\n");
    printf("4. Remover Música\n");
    printf("5. Listar Playlists\n");
    printf("6. Buscar no Catálogo\n");
    printf("7. Ordenar Playlist\n");
    printf("8. Estatísticas\n");
    printf("9. Exportar Playlist para TXT\n");
    printf("10. Salvar e Sair\n");
    printf("0. Sair sem Salvar\n");
    printf("=========================================\n");
}

void executar_opcao_criar_playlist(SistemaPlaylists* sistema) {
    char nome[MAX];
    printf("Nome da nova playlist: ");
    fgets(nome, MAX, stdin);
    nome[strcspn(nome, "\n")] = 0;
    
    if (strlen(nome) == 0) {
        printf("❌ Nome não pode ser vazio!\n");
        pausar_tela();
        return;
    }
    
    if (adicionar_playlist(sistema, nome)) {
        printf("✅ Playlist '%s' criada com sucesso!\n", nome);
    } else {
        printf("❌ Erro ao criar playlist.\n");
    }
    
    pausar_tela();
}

void executar_opcao_adicionar_musica_catalogo(SistemaPlaylists* sistema, CatalogoMusicas* catalogo) {
    if (!catalogo || catalogo->qtdMusicas == 0) {
        printf("❌ Catálogo não disponível ou vazio!\n");
        pausar_tela();
        return;
    }
    
    if (sistema->qtdPlaylists == 0) {
        printf("❌ Crie uma playlist primeiro!\n");
        pausar_tela();
        return;
    }
    
    printf("🔍 Buscar música no catálogo:\n");
    printf("1. Por título\n");
    printf("2. Por artista\n");
    printf("3. Por gênero\n");
    printf("Opção: ");
    
    int tipo_busca;
    if (scanf("%d", &tipo_busca) != 1 || tipo_busca < 1 || tipo_busca > 3) {
        printf("❌ Opção inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();
    
    char termo[MAX];
    const char* termos[] = {"título", "artista", "gênero"};
    printf("Digite o %s: ", termos[tipo_busca-1]);
    fgets(termo, MAX, stdin);
    termo[strcspn(termo, "\n")] = 0;
    
    if (strlen(termo) == 0) {
        printf("❌ Termo de busca não pode ser vazio!\n");
        pausar_tela();
        return;
    }
    
    int resultados[50];
    int encontrados = buscar_musica_catalogo(catalogo, termo, tipo_busca, resultados, 50);
    
    if (encontrados == 0) {
        printf("❌ Nenhuma música encontrada.\n");
        pausar_tela();
        return;
    }
    
    printf("\n🎵 Músicas encontradas (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        const Musica* m = &catalogo->musicas[resultados[i]];
        printf("%d. %s - %s (%s, %d segundos)\n", 
               i + 1, m->titulo, m->artista, m->genero, m->duracao);
    }
    
    printf("\nSelecione a música (1-%d) ou 0 para cancelar: ", encontrados);
    int escolha;
    if (scanf("%d", &escolha) != 1 || escolha < 0 || escolha > encontrados) {
        printf("❌ Escolha inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    if (escolha == 0) {
        printf("Operação cancelada.\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    // Selecionar playlist para adicionar
    printf("\n📁 Selecione a playlist para adicionar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }
    
    int playlist_idx;
    printf("Playlist número: ");
    if (scanf("%d", &playlist_idx) != 1 || 
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("❌ Playlist inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    // Adiciona a música selecionada
    int musica_idx = resultados[escolha - 1];
    const Musica* musica_selecionada = &catalogo->musicas[musica_idx];
    
    if (adicionar_musica_playlist(&sistema->playlists[playlist_idx - 1], musica_selecionada)) {
        printf("✅ Música '%s' adicionada à playlist '%s'!\n", 
               musica_selecionada->titulo, 
               sistema->playlists[playlist_idx - 1].nome);
    } else {
        printf("❌ Erro ao adicionar música.\n");
    }
    
    limpar_buffer();
    pausar_tela();
}

void executar_opcao_adicionar_musica_manual(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("❌ Crie uma playlist primeiro!\n");
        pausar_tela();
        return;
    }
    
    printf("📁 Selecione a playlist:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }
    
    int playlist_idx;
    printf("Playlist número: ");
    if (scanf("%d", &playlist_idx) != 1 || 
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("❌ Playlist inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();
    
    Musica m;
    printf("Título: ");
    fgets(m.titulo, MAX, stdin);
    m.titulo[strcspn(m.titulo, "\n")] = 0;
    
    printf("Artista: ");
    fgets(m.artista, MAX, stdin);
    m.artista[strcspn(m.artista, "\n")] = 0;
    
    printf("Gênero: ");
    fgets(m.genero, MAX, stdin);
    m.genero[strcspn(m.genero, "\n")] = 0;
    
    printf("Duração (segundos): ");
    if (scanf("%d", &m.duracao) != 1 || m.duracao <= 0) {
        printf("Duração inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    printf("Informação extra:\n");
    printf("1. Ano de lançamento\n");
    printf("2. Número do álbum\n");
    printf("Opção: ");
    if (scanf("%d", &m.tipoInfoExtra) != 1 || (m.tipoInfoExtra != 1 && m.tipoInfoExtra != 2)) {
        printf("Opção inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }

    if (m.tipoInfoExtra == 1) {
        printf("Ano: ");
        if (scanf("%d", &m.extra.anoLancamento) != 1) {
            printf("Ano inválido!\n");
            limpar_buffer();
            pausar_tela();
            return;
        }
        } else {
            printf("Número do álbum: ");
            if (scanf("%d", &m.extra.numeroAlbum) != 1) {
                printf("Número do álbum inválido!\n");
                limpar_buffer();
                pausar_tela();
                return;
            }
        }
    
    if (adicionar_musica_playlist(&sistema->playlists[playlist_idx - 1], &m)) {
        printf("✅ Música adicionada com sucesso!\n");
    } else {
        printf("❌ Erro ao adicionar música.\n");
    }
    
    limpar_buffer();
    pausar_tela();
}

void executar_opcao_remover_musica(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("❌ Nenhuma playlist disponível!\n");
        pausar_tela();
        return;
    }
    
    printf("📁 Selecione a playlist:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s (%d música%s)\n", 
               i + 1, 
               sistema->playlists[i].nome,
               sistema->playlists[i].qtdMusicas,
               sistema->playlists[i].qtdMusicas != 1 ? "s" : "");
    }
    
    int playlist_idx;
    printf("Playlist número: ");
    if (scanf("%d", &playlist_idx) != 1 || 
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("❌ Playlist inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    Playlist* playlist = &sistema->playlists[playlist_idx - 1];
    
    if (playlist->qtdMusicas == 0) {
        printf("❌ Esta playlist está vazia!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    printf("\n🎵 Músicas na playlist '%s':\n", playlist->nome);
    for (int i = 0; i < playlist->qtdMusicas; i++) {
        printf("%d. %s - %s\n", 
               i + 1, 
               playlist->musicas[i].titulo,
               playlist->musicas[i].artista);
    }
    
    int musica_idx;
    printf("\nMúsica número a remover: ");
    if (scanf("%d", &musica_idx) != 1 || 
        musica_idx < 1 || musica_idx > playlist->qtdMusicas) {
        printf("❌ Música inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    printf("⚠️  Remover '%s'? (s/N): ", playlist->musicas[musica_idx - 1].titulo);
    char confirmacao;
    limpar_buffer();
    scanf("%c", &confirmacao);
    
    if (tolower(confirmacao) == 's') {
        if (remover_musica_playlist(playlist, musica_idx - 1)) {
            printf("✅ Música removida com sucesso!\n");
        } else {
            printf("❌ Erro ao remover música.\n");
        }
    } else {
        printf("Operação cancelada.\n");
    }
    
    limpar_buffer();
    pausar_tela();
}

void executar_opcao_listar_playlists(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("📭 Nenhuma playlist criada.\n");
        pausar_tela();
        return;
    }
    
    printf("📚 PLAYLISTS DISPONÍVEIS:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        Playlist* p = &sistema->playlists[i];
        
        int horas = p->duracaoTotal / 3600;
        int minutos = (p->duracaoTotal % 3600) / 60;
        int segundos = p->duracaoTotal % 60;
        
        printf("\n📁 %s\n", p->nome);
        printf("   🎵 Músicas: %d\n", p->qtdMusicas);
        
        if (horas > 0) {
            printf("   ⏱️  Duração total: %02d:%02d:%02d\n", horas, minutos, segundos);
        } else {
            printf("   ⏱️  Duração total: %02d:%02d\n", minutos, segundos);
        }
        
        if (p->qtdMusicas > 0) {
            printf("   📋 Lista de músicas:\n");
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
        printf("❌ Catálogo não disponível!\n");
        pausar_tela();
        return;
    }
    
    printf("🔍 Buscar no catálogo:\n");
    printf("1. Por título\n");
    printf("2. Por artista\n");
    printf("3. Por gênero\n");
    printf("Opção: ");
    
    int tipo_busca;
    if (scanf("%d", &tipo_busca) != 1 || tipo_busca < 1 || tipo_busca > 3) {
        printf("❌ Opção inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    limpar_buffer();
    
    char termo[MAX];
    const char* termos[] = {"título", "artista", "gênero"};
    printf("Digite o %s: ", termos[tipo_busca-1]);
    fgets(termo, MAX, stdin);
    termo[strcspn(termo, "\n")] = 0;
    
    if (strlen(termo) == 0) {
        printf("❌ Termo de busca não pode ser vazio!\n");
        pausar_tela();
        return;
    }
    
    int resultados[100];
    int encontrados = buscar_musica_catalogo(catalogo, termo, tipo_busca, resultados, 100);
    
    if (encontrados == 0) {
        printf("❌ Nenhuma música encontrada.\n");
    } else {
        printf("\n🎵 Resultados da busca (%d encontrados):\n", encontrados);
        for (int i = 0; i < encontrados; i++) {
            const Musica* m = &catalogo->musicas[resultados[i]];
            int min = m->duracao / 60;
            int seg = m->duracao % 60;
            
            printf("%d. %s\n", i + 1, m->titulo);
            printf("   Artista: %s\n", m->artista);
            printf("   Gênero: %s\n", m->genero);
            printf("   Duração: %02d:%02d\n", min, seg);
            
            if (m->tipoInfoExtra == 1) {
                printf("   Ano: %d\n", m->extra.anoLancamento);
            } else {
                printf("   Álbum: %d\n", m->extra.numeroAlbum);
            }
            
            if (i < encontrados - 1) printf("\n");
        }
    }
    
    pausar_tela();
}

int comparar_titulos(const Musica* a, const Musica* b) {
    return strcasecmp(a->titulo, b->titulo);
}

int comparar_duracao(const Musica* a, const Musica* b) {
    return a->duracao - b->duracao;
}

void bubble_sort(Musica* vetor, int n, int (*comparar)(const Musica*, const Musica*)) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (comparar(&vetor[j], &vetor[j+1]) > 0) {
                Musica temp = vetor[j];
                vetor[j] = vetor[j+1];
                vetor[j+1] = temp;
            }
        }
    }
}

void executar_opcao_ordenar_playlist(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("❌ Nenhuma playlist disponível!\n");
        pausar_tela();
        return;
    }
    
    printf("📁 Selecione a playlist para ordenar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s\n", i + 1, sistema->playlists[i].nome);
    }
    
    int playlist_idx;
    printf("Playlist número: ");
    if (scanf("%d", &playlist_idx) != 1 || 
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("❌ Playlist inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    Playlist* playlist = &sistema->playlists[playlist_idx - 1];
    
    if (playlist->qtdMusicas < 2) {
        printf("❌ A playlist precisa ter pelo menos 2 músicas!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    printf("🔢 Critério de ordenação:\n");
    printf("1. Título (A-Z)\n");
    printf("2. Duração (menor para maior)\n");
    printf("Opção: ");
    
    int criterio;
    if (scanf("%d", &criterio) != 1 || (criterio != 1 && criterio != 2)) {
        printf("❌ Critério inválido!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    if (criterio == 1) {
        bubble_sort(playlist->musicas, playlist->qtdMusicas, comparar_titulos);
        printf("✅ Playlist ordenada por título!\n");
    } else {
        bubble_sort(playlist->musicas, playlist->qtdMusicas, comparar_duracao);
        printf("✅ Playlist ordenada por duração!\n");
    }
    
    limpar_buffer();
    pausar_tela();
}

void executar_opcao_estatisticas(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("📭 Nenhuma playlist criada.\n");
        pausar_tela();
        return;
    }
    
    printf("📊 ESTATÍSTICAS DO SISTEMA:\n");
    
    int total_musicas = 0;
    int total_duracao = 0;
    Playlist* maior_playlist = &sistema->playlists[0];
    Playlist* menor_playlist = &sistema->playlists[0];
    
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        Playlist* p = &sistema->playlists[i];
        
        total_musicas += p->qtdMusicas;
        total_duracao += p->duracaoTotal;
        
        if (p->qtdMusicas > maior_playlist->qtdMusicas) {
            maior_playlist = p;
        }
        if (p->qtdMusicas < menor_playlist->qtdMusicas) {
            menor_playlist = p;
        }
    }
    
    int horas_total = total_duracao / 3600;
    int minutos_total = (total_duracao % 3600) / 60;
    
    printf("📁 Total de playlists: %d\n", sistema->qtdPlaylists);
    printf("🎵 Total de músicas: %d\n", total_musicas);
    
    if (horas_total > 0) {
        printf("⏱️  Duração total: %02d:%02d:%02d\n", 
               horas_total, minutos_total, total_duracao % 60);
    } else {
        printf("⏱️  Duração total: %02d:%02d\n", 
               minutos_total, total_duracao % 60);
    }
    
    if (total_musicas > 0) {
        float media_por_playlist = (float)total_musicas / sistema->qtdPlaylists;
        float media_duracao = (float)total_duracao / total_musicas;
        
        printf("📈 Média de músicas por playlist: %.1f\n", media_por_playlist);
        printf("⏳ Duração média por música: %.0f segundos\n", media_duracao);
    }
    
    printf("\n🏆 Playlist com mais músicas: %s (%d música%s)\n",
           maior_playlist->nome,
           maior_playlist->qtdMusicas,
           maior_playlist->qtdMusicas != 1 ? "s" : "");
    
    printf("📉 Playlist com menos músicas: %s (%d música%s)\n",
           menor_playlist->nome,
           menor_playlist->qtdMusicas,
           menor_playlist->qtdMusicas != 1 ? "s" : "");
    
    pausar_tela();
}

void executar_opcao_exportar_playlist(SistemaPlaylists* sistema) {
    if (sistema->qtdPlaylists == 0) {
        printf("❌ Nenhuma playlist disponível!\n");
        pausar_tela();
        return;
    }
    
    printf("📁 Selecione a playlist para exportar:\n");
    for (int i = 0; i < sistema->qtdPlaylists; i++) {
        printf("%d. %s (%d música%s)\n", 
               i + 1, 
               sistema->playlists[i].nome,
               sistema->playlists[i].qtdMusicas,
               sistema->playlists[i].qtdMusicas != 1 ? "s" : "");
    }
    
    int playlist_idx;
    printf("Playlist número: ");
    if (scanf("%d", &playlist_idx) != 1 || 
        playlist_idx < 1 || playlist_idx > sistema->qtdPlaylists) {
        printf("❌ Playlist inválida!\n");
        limpar_buffer();
        pausar_tela();
        return;
    }
    
    char nome_arquivo[MAX];
    printf("Nome do arquivo para exportação (ex: minha_playlist.txt): ");
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

void executar_menu_principal(SistemaPlaylists* sistema, CatalogoMusicas* catalogo) {
    int executando = 1;
    
    while (executando) {
        mostrar_menu_principal();
        int opcao = obter_opcao_menu(0, 10);
        
        switch (opcao) {
            case OP_CRIAR_PLAYLIST:
                executar_opcao_criar_playlist(sistema);
                break;
            case OP_ADICIONAR_MUSICA_CATALOGO:
                executar_opcao_adicionar_musica_catalogo(sistema, catalogo);
                break;
            case OP_ADICIONAR_MUSICA_MANUAL:
                executar_opcao_adicionar_musica_manual(sistema);
                break;
            case OP_REMOVER_MUSICA:
                executar_opcao_remover_musica(sistema);
                break;
            case OP_LISTAR_PLAYLISTS:
                executar_opcao_listar_playlists(sistema);
                break;
            case OP_BUSCAR_CATALOGO:
                executar_opcao_buscar_catalogo(catalogo);
                break;
            case OP_ORDENAR_PLAYLIST:
                executar_opcao_ordenar_playlist(sistema);
                break;
            case OP_ESTATISTICAS:
                executar_opcao_estatisticas(sistema);
                break;
            case OP_EXPORTAR_PLAYLIST:
                executar_opcao_exportar_playlist(sistema);
                break;
            case OP_SALVAR_SAIR:
                salvar_sistema_arquivo(sistema);
                printf("💾 Dados salvos. Saindo...\n");
                executando = 0;
                break;
            case OP_SAIR_SEM_SALVAR:
                printf("⚠️  Saindo sem salvar...\n");
                executando = 0;
                break;
            default:
                printf("❌ Opção inválida! Tente novamente.\n");
                limpar_buffer();
                pausar_tela();
                break;
        }
    }
}
