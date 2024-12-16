#ifndef _EPIDEMIA_H
#define _EPIDEMIA_H

#define NUM_USUARIOS_MAX 6
#define NUM_AMIGOS_PERMITIDOS NUM_USUARIOS_MAX
#define MAX_STRING 255
#define REWIRE_PROBABILITY 0.2  // Probabilidade de reorganizar conexões
#define TAXA_TRANSMISSAO 0.5   // Probabilidade de transmitir a infecção
#define TAXA_RECUPERACAO 0.5   // Probabilidade de se curar
#define TAXA_MORTALIDADE 0.2   // Probabilidade de morrer
#define DIAS_PARA_CONTAMINAR 2 // Dias para se tornar contaminante

typedef enum {
    SAUDAVEL,
    INFECTADO,
    CONTAMINANTE,
    CURADO,
    MORTO
} Estado;

typedef struct usuario {
    unsigned int id;
    char nome[MAX_STRING];
} Usuario;

typedef struct nodo {
    struct nodo **conexoes_de_amigos;
    Usuario *usuario;
    int num_amigos;
    Estado estado;
    int dias_infectado;
} Graph;

// Função para gerar uma representação no Graphviz
typedef struct {
    const char *cor;
} EstadoCor;

typedef struct epidemia{
    Graph** grafo;
    int atual_infectados;
    int atual_contaminantes;
    int atual_curados;
    int atual_mortos;

    int total_infectados;
    int total_contaminantes;
} Epidemia;

Epidemia *criar_rede_livre_escala(int num_vertices, int num_arestas_iniciais);
Epidemia *criar_rede_aleatoria(int num_vertices, float probabilidade);
Epidemia *criar_pequenos_mundos(int num_vertices, int k, float probabilidade);
void propagar_infeccao(Epidemia *epidemia, int num_vertices);
void simular_epidemia(Epidemia *epidemia, int num_vertices, int num_iteracoes, bool gerar_imagens_do_grafo);
void atualizar_estados(Epidemia *epidemia, int num_vertices);

void imprimir_grafo(Graph **grafo, int num_vertices);
void gerar_graphviz(Graph **grafo, int num_vertices, const char *arquivo_saida);

char* getDateAndTimeString();
char* getMillisseconds();

void gravar_informacoes(int iteracao, int num_interacoes, int num_vertices, Epidemia* epidemia, bool gerar_imagens_do_grafo);
void gerar_csv(Epidemia* epidemia, char* pasta_csv_simulacao, int pico_infectados, int tempo_ate_pico, int num_iteracoes, int num_vertices, char* nome_arquivo_plot_infectados);

#endif