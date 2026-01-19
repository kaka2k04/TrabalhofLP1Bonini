# Sistema de Gerenciamento de Playlists em C

## Descrição
Sistema desenvolvido em linguagem C para gerenciamento de playlists musicais. Permite criar, editar, organizar e exportar playlists, com suporte a catálogo de músicas pré-carregado e persistência de dados.

---

## Como Executar

### Requisitos

- Um compilador C (como GCC ou Clang):
  - **Linux:** `sudo apt install build-essential`
  - **Windows:** Instale o MinGW.
  - **macOS:** Instale o Xcode Command Line Tools com `xcode-select --install`.

### Passos

1. Clone este repositório:

```bash
git clone https://github.com/kaka2k04/TrabalhofLP1Bonini.git
```

```bash
cd TrabalhofLP1Bonini
```

2. Compile o código:

```bash
gcc main.c menu.c operations.c playlist.c catalogo.c storage.c util.c -I./include -o playlist_system
```

3. Execute o programa:

```bash
./playlist_system
```

---

## Estrutura do Código

O código está organizado em vários arquivos:

### Arquivos Principais:
- **`main.c`** - Contém a função principal do programa e inicializa o sistema.
- **`menu.c`** - Gerencia a exibição e execução do menu principal.

### Headers:
- **`core.h`** - Define todas as estruturas de dados (structs) e protótipos de funções principais.
- **`operations.h`** - Define as operações do sistema e constantes do menu.

### Implementações:
- **`operations.c`** - Implementa todas as operações do sistema.
- **`playlist.c`** - Implementa funções para criar, gerenciar e liberar playlists.
- **`catalogo.c`** - Carrega o catálogo de músicas do arquivo e implementa busca e exportação.
- **`storage.c`** - Gerencia a persistência de dados (salvar/carregar playlists).
- **`util.c`** - Funções auxiliares (limpar buffer, pausar tela, etc.).

### Arquivos de Dados:
- **`catalogo_musicas.txt`** - Arquivo de texto com o catálogo inicial de músicas.
- **`playlists.dat`** - Arquivo binário onde as playlists são salvas (criado automaticamente).
- **`playlist_exportada.txt`** - Arquivo gerado ao exportar uma playlist.

---

## Funcionalidades

### Sistema de Playlists (`playlist.h` e `playlist.c`)

```c
typedef struct {
    char nome[MAX];
    Musica *musicas;
    int qtdMusicas;
    int capacidade;
    int duracaoTotal;
} Playlist;
```

```c
SistemaPlaylists* criar_sistema();
int adicionar_playlist(SistemaPlaylists* sistema, const char* nome);
int adicionar_musica_playlist(Playlist* playlist, const Musica* musica);
int remover_musica_playlist(Playlist* playlist, int indice);
```

**Funcionamento:**
1. Cria sistema de gerenciamento de playlists
2. Adiciona novas playlists com nomes personalizados
3. Gerencia músicas dentro de cada playlist
4. Remove músicas específicas quando necessário

### Catálogo de Músicas (`catalogo.c`)

```c
CatalogoMusicas* carregar_catalogo_musicas();
int buscar_musica_catalogo(const CatalogoMusicas* catalogo, const char* termo, 
                          int tipo_busca, int* resultados, int max_resultados);
```

**Funcionamento:**
1. Carrega catálogo de músicas a partir de arquivo texto
2. Realiza buscas por título, artista ou gênero (case-insensitive)
3. Retorna lista de resultados encontrados

### Persistência de Dados (`storage.c`)

```c
void salvar_sistema_arquivo(const SistemaPlaylists* sistema);
void carregar_sistema_arquivo(SistemaPlaylists* sistema);
```

**Funcionamento:**
1. Salva todas as playlists em arquivo binário
2. Carrega playlists salvas automaticamente ao iniciar
3. Mantém os dados entre execuções do programa

### Interface do Usuário (`main.c` e `menu.c`)

**Fluxo Principal:**
1. Exibe menu com 10 opções de operação
2. Processa a escolha do usuário
3. Chama a função correspondente à operação selecionada
4. Retorna ao menu após cada operação

---

## Como funciona o programa

### Menu Principal
```
=== SISTEMA DE PLAYLISTS COM CATALOGO ===
1. Criar Playlist
2. Adicionar Música do Catálogo
3. Adicionar Música Manualmente
4. Remover Música
5. Listar Playlists
6. Buscar no Catalogo
7. Ordenar Playlist
8. Estatísticas
9. Exportar Playlist para TXT
10. Salvar e Sair
0. Sair sem Salvar
========================================
```

### Fluxo de Uso:
1. **Inicialização**: O programa carrega automaticamente playlists salvas e o catálogo de músicas
2. **Criação**: Usuário cria playlists com nomes personalizados
3. **Adição**: Músicas podem ser adicionadas do catálogo ou manualmente
4. **Gerenciamento**: Músicas podem ser removidas, playlists ordenadas
5. **Consulta**: Usuário pode buscar no catálogo e ver estatísticas
6. **Exportação**: Playlists podem ser exportadas para arquivos de texto
7. **Persistência**: Dados são salvos automaticamente ao sair

### Formato do Arquivo de Catálogo:
```
Título|Artista|Gênero|Duração|ValorExtra|TipoExtra
Exemplo: Bohemian Rhapsody|Queen|Rock|354|1975|1
```
- `TipoExtra = 1` → Ano de lançamento
- `TipoExtra = 2` → Número do álbum

---

## Exemplo de Saída no Terminal

### Criação de Playlist
```
=== SISTEMA DE PLAYLISTS COM CATALOGO ===
Opcao: 1
Nome da nova playlist: Rock Classico
Sua Playlist 'Rock Classico' foi criada com sucesso!
```

### Adição de Música do Catálogo
```
Opcao: 2
Buscar musica no catalogo:
1. Por titulo
2. Por artista
3. Por genero
Opcao: 3
Digite o genero: Rock
Musicas encontradas (8):
1. Bohemian Rhapsody - Queen (Rock, 354 segundos)
2. Stairway to Heaven - Led Zeppelin (Rock, 482 segundos)
...
Selecione a musica (1 a 8) ou 0 para cancelar: 1
Selecione a playlist para adicionar:
1. Rock Classico
Playlist numero: 1
Musica 'Bohemian Rhapsody' adicionada a playlist 'Rock Classico'!
```

### Estatísticas do Sistema
```
Opcao: 8
ESTATISTICAS DO SISTEMA:
Total de playlists: 3
Total de musicas: 15
Duracao total: 02:15:30
Media de musicas por playlist: 5.0
Duracao media por musica: 543 segundos

Playlist com mais musicas: Rock Classico (7 musicas)
Playlist com menos musicas: Jazz Relax (3 musicas)
```

### Exportação de Playlist
```
Opcao: 9
Selecione a playlist para exportar:
1. Rock Classico (5 musicas)
Playlist numero: 1
Nome do arquivo para exportacao (ex: minha_playlist.txt): rock_classico.txt
Playlist exportada para: rock_classico.txt
```

### Saída com Salvamento
```
Opcao: 10
Dados salvos. Saindo...
Programa finalizado.
```

---

## Tecnologias Utilizadas

- Linguagem C
- Biblioteca padrão (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`)
- Manipulação de arquivos (texto e binário)
- Alocação dinâmica de memória
- Structs e unions para organização de dados

---

## Contribuição

Sinta-se à vontade para contribuir com melhorias neste projeto. Sugestões, correções e ideias são bem-vindas!

---

## Licença

Este projeto é de uso livre para fins educacionais.

---

## Autores

Amadeu Fernandes, Carlos Eduardo e Sarah Victoria
