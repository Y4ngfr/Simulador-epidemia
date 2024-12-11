#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "grafo.h"

Graph **criar_rede_livre_escala(int num_vertices, int num_arestas_iniciais) {
    if (num_arestas_iniciais > num_vertices) {
        fprintf(stderr, "Erro: Número de arestas iniciais deve ser menor ou igual ao número total de vértices.\n");
        exit(EXIT_FAILURE);
    }

    Graph **grafo = malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        perror("Erro ao alocar grafo");
        exit(EXIT_FAILURE);
    }

    // Inicializa os nós
    for (int i = 0; i < num_vertices; i++) {
        grafo[i] = malloc(sizeof(Graph));
        grafo[i]->usuario = malloc(sizeof(Usuario));
        grafo[i]->conexoes_de_amigos = malloc(num_vertices * sizeof(Graph *)); // Tamanho máximo
        grafo[i]->num_amigos = 0;
        grafo[i]->estado = SAUDAVEL;
        grafo[i]->dias_infectado = 0;
        grafo[i]->usuario->id = i;
        snprintf(grafo[i]->usuario->nome, MAX_STRING, "Usuario_%d", i);
    }

    // Conexões iniciais completas entre os primeiros `num_arestas_iniciais` nós
    for (int i = 0; i < num_arestas_iniciais; i++) {
        for (int j = i + 1; j < num_arestas_iniciais; j++) {
            grafo[i]->conexoes_de_amigos[grafo[i]->num_amigos++] = grafo[j];
            grafo[j]->conexoes_de_amigos[grafo[j]->num_amigos++] = grafo[i];
        }
    }

    // Adiciona novos nós ao grafo, conectando com base na probabilidade proporcional ao grau
    for (int i = num_arestas_iniciais; i < num_vertices; i++) {
        int arestas_adicionadas = 0;

        while (arestas_adicionadas < num_arestas_iniciais) {
            int alvo = rand() % i; // Escolhe um nó entre os já existentes
            bool ja_conectado = false;

            // Verifica se já há conexão
            for (int j = 0; j < grafo[i]->num_amigos; j++) {
                if (grafo[i]->conexoes_de_amigos[j]->usuario->id == alvo) {
                    ja_conectado = true;
                    break;
                }
            }

            if (!ja_conectado) {
                // Conecta os nós
                grafo[i]->conexoes_de_amigos[grafo[i]->num_amigos++] = grafo[alvo];
                grafo[alvo]->conexoes_de_amigos[grafo[alvo]->num_amigos++] = grafo[i];
                arestas_adicionadas++;
            }
        }
    }

    return grafo;
}

Graph **criar_rede_aleatoria(int num_vertices, float probabilidade) {
    Graph **grafo = malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        perror("Erro ao alocar grafo");
        exit(EXIT_FAILURE);
    }

    // Inicializa os nós
    for (int i = 0; i < num_vertices; i++) {
        grafo[i] = malloc(sizeof(Graph));
        grafo[i]->usuario = malloc(sizeof(Usuario));
        grafo[i]->conexoes_de_amigos = malloc(num_vertices * sizeof(Graph *)); // Tamanho máximo
        grafo[i]->num_amigos = 0;
        grafo[i]->estado = SAUDAVEL;
        grafo[i]->dias_infectado = 0;
        grafo[i]->usuario->id = i;
        snprintf(grafo[i]->usuario->nome, MAX_STRING, "Usuario_%d", i);
    }

    // Adiciona arestas de forma aleatória
    for (int i = 0; i < num_vertices; i++) {
        for (int j = i + 1; j < num_vertices; j++) { // Evita duplicatas (i, j) e (j, i)
            if (((float)rand() / RAND_MAX) < probabilidade) {
                grafo[i]->conexoes_de_amigos[grafo[i]->num_amigos++] = grafo[j];
                grafo[j]->conexoes_de_amigos[grafo[j]->num_amigos++] = grafo[i];
            }
        }
    }

    return grafo;
}

Graph **criar_pequenos_mundos(int num_vertices, int k, float probabilidade) {
    Graph **grafo = malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        perror("Erro ao alocar grafo");
        exit(EXIT_FAILURE);
    }

    // Inicializa os nós
    for (int i = 0; i < num_vertices; i++) {
        grafo[i] = malloc(sizeof(Graph));
        grafo[i]->usuario = malloc(sizeof(Usuario));
        grafo[i]->conexoes_de_amigos = malloc(k * sizeof(Graph *));
        grafo[i]->num_amigos = 0;
        grafo[i]->estado = SAUDAVEL;
        grafo[i]->dias_infectado = 0;
        grafo[i]->usuario->id = i;
        snprintf(grafo[i]->usuario->nome, MAX_STRING, "Usuario_%d", i);
    }

    // Conecta os nós em uma rede circular
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 1; j <= k / 2; j++) {
            int vizinho = (i + j) % num_vertices;
            grafo[i]->conexoes_de_amigos[grafo[i]->num_amigos++] = grafo[vizinho];
            grafo[vizinho]->conexoes_de_amigos[grafo[vizinho]->num_amigos++] = grafo[i];
        }
    }

    // Reorganiza conexões com probabilidade
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < grafo[i]->num_amigos; j++) {
            if (((float)rand() / RAND_MAX) < probabilidade) {
                int novo_vizinho;
                do {
                    novo_vizinho = rand() % num_vertices;
                } while (novo_vizinho == i || novo_vizinho == grafo[i]->conexoes_de_amigos[j]->usuario->id);

                // Substitui a conexão
                grafo[i]->conexoes_de_amigos[j] = grafo[novo_vizinho];
            }
        }
    }

    return grafo;
}

void imprimir_grafo(Graph **grafo, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        printf("Usuario %d (%s): ", grafo[i]->usuario->id, grafo[i]->usuario->nome);
        for (int j = 0; j < grafo[i]->num_amigos; j++) {
            printf("%d ", grafo[i]->conexoes_de_amigos[j]->usuario->id);
        }
        printf("\n");
    }
}

void gerar_graphviz(Graph **grafo, int num_vertices, const char *arquivo_saida) {
    EstadoCor cores[] = {
        {"green"},     // SAUDAVEL
        {"yellow"},    // INFECTADO
        {"orange"},    // CONTAMINANTE
        {"red"},       // DOENTE
        {"blue"},      // CURADO
        {"black"}      // MORTO
    };

    FILE *arquivo = fopen(arquivo_saida, "w");
    if (!arquivo) {
        perror("Erro ao criar arquivo Graphviz");
        exit(EXIT_FAILURE);
    }

    fprintf(arquivo, "graph Epidemia {\n");
    fprintf(arquivo, "    layout=circo;\n");
    fprintf(arquivo, "    node [style=filled];\n");

    // fprintf(arquivo, "graph Epidemia {\n");
    // fprintf(arquivo, "    node [style=filled];\n");

    // Adiciona nós com cores
    for (int i = 0; i < num_vertices; i++) {
        fprintf(arquivo, "    %d [label=\"%s\" color=%s];\n",
                grafo[i]->usuario->id,
                grafo[i]->usuario->nome,
                cores[grafo[i]->estado].cor);
    }

    // Adiciona arestas
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < grafo[i]->num_amigos; j++) {
            if (grafo[i]->usuario->id < grafo[i]->conexoes_de_amigos[j]->usuario->id) {
                fprintf(arquivo, "    %d -- %d;\n",
                        grafo[i]->usuario->id,
                        grafo[i]->conexoes_de_amigos[j]->usuario->id);
            }
        }
    }

    fprintf(arquivo, "}\n");
    fclose(arquivo);
}

void atualizar_estados(Graph **grafo, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        Graph *nodo = grafo[i];

        switch (nodo->estado) {
            case INFECTADO:
                nodo->dias_infectado++;
                if (nodo->dias_infectado >= DIAS_PARA_CONTAMINAR) {
                    nodo->estado = CONTAMINANTE;
                }
                break;

            case CONTAMINANTE:
                if ((float)rand() / RAND_MAX < TAXA_RECUPERACAO) {
                    nodo->estado = CURADO;
                } else if ((float)rand() / RAND_MAX < TAXA_MORTALIDADE) {
                    nodo->estado = MORTO;
                }
                break;

            case DOENTE:
                if ((float)rand() / RAND_MAX < TAXA_RECUPERACAO) {
                    nodo->estado = CURADO;
                } else if ((float)rand() / RAND_MAX < TAXA_MORTALIDADE) {
                    nodo->estado = MORTO;
                }
                break;

            default:
                break;
        }
    }
}

void propagar_infeccao(Graph **grafo, int num_vertices) {
    for (int i = 0; i < num_vertices; i++) {
        Graph *nodo = grafo[i];

        if (nodo->estado == CONTAMINANTE) {
            for (int j = 0; j < nodo->num_amigos; j++) {
                Graph *vizinho = nodo->conexoes_de_amigos[j];
                if (vizinho->estado == SAUDAVEL && (float)rand() / RAND_MAX < TAXA_TRANSMISSAO) {
                    vizinho->estado = INFECTADO;
                }
            }
        }
    }
}

void simular_epidemia(Graph **grafo, int num_vertices, int num_iteracoes) {
    // Infecta o primeiro nó para iniciar a epidemia
    grafo[0]->estado = INFECTADO;

    for (int iteracao = 0; iteracao < num_iteracoes; iteracao++) {
        printf("Iteração %d:\n", iteracao);
        imprimir_grafo(grafo, num_vertices);

        // Atualiza os estados e propaga a infecção
        atualizar_estados(grafo, num_vertices);
        propagar_infeccao(grafo, num_vertices);

        // Gera arquivo Graphviz para cada iteração
        char nome_arquivo[100], command[200];
        snprintf(nome_arquivo, 100, "output_dot/epidemia_iteracao_%d.dot", iteracao);
        snprintf(command, 200, "dot %s -Tsvg > output_svg/%.*s.svg", nome_arquivo, (int)(strlen(nome_arquivo) - 14), &(nome_arquivo[10]));
        gerar_graphviz(grafo, num_vertices, nome_arquivo);
        system(command);
    }
}