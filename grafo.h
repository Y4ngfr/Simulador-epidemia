#ifndef _GRAFO_H
#define _GRAFO_H

#define NUM_USUARIOS_MAX 6
#define NUM_AMIGOS_PERMITIDOS NUM_USUARIOS_MAX
#define MAX_STRING 255
#define REWIRE_PROBABILITY 0.3  // Probabilidade de reorganizar conexões
#define TAXA_TRANSMISSAO 0.3   // Probabilidade de transmitir a infecção
#define TAXA_RECUPERACAO 0.4   // Probabilidade de se curar
#define TAXA_MORTALIDADE 0.1   // Probabilidade de morrer
#define DIAS_PARA_CONTAMINAR 2 // Dias para se tornar contaminante

typedef enum {
    SAUDAVEL,
    INFECTADO,
    CONTAMINANTE,
    DOENTE,
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


Graph **criar_rede_livre_escala(int num_vertices, int num_arestas_iniciais);
Graph **criar_rede_livre_escala(int num_vertices, int num_arestas_iniciais);
Graph **criar_pequenos_mundos(int num_vertices, int k, float probabilidade);
void imprimir_grafo(Graph **grafo, int num_vertices);
void propagar_infeccao(Graph **grafo, int num_vertices);
void simular_epidemia(Graph **grafo, int num_vertices, int num_iteracoes);
void atualizar_estados(Graph **grafo, int num_vertices);
void gerar_graphviz(Graph **grafo, int num_vertices, const char *arquivo_saida);

#endif