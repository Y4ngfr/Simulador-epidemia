#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "epidemia.h"

Epidemia *criar_rede_livre_escala(int num_vertices, int num_arestas_iniciais) {
    if (num_arestas_iniciais > num_vertices) {
        fprintf(stderr, "Erro: Número de arestas iniciais deve ser menor ou igual ao número total de vértices.\n");
        exit(EXIT_FAILURE);
    }

    Epidemia* epidemia = (Epidemia*)malloc(sizeof(Epidemia));
    if(epidemia == NULL){
        perror("Erro ao alocar epidemia");
        exit(EXIT_FAILURE);
    }

    Graph **grafo = malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        free(epidemia);
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

    epidemia->grafo = grafo;
    epidemia->curados = 0;
    epidemia->doentes = 0;
    epidemia->infectados = 0;
    epidemia->mortos = 0;
    epidemia->contaminantes = 0;

    return epidemia;
}

Epidemia *criar_rede_aleatoria(int num_vertices, float probabilidade) {
    Epidemia* epidemia = (Epidemia*)malloc(sizeof(Epidemia));
    if(epidemia == NULL){
        perror("Erro ao alocar epidemia");
        exit(EXIT_FAILURE);
    }

    Graph **grafo = malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        free(epidemia);
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

    epidemia->grafo = grafo;
    epidemia->curados = 0;
    epidemia->doentes = 0;
    epidemia->infectados = 0;
    epidemia->mortos = 0;
    epidemia->contaminantes = 0;

    return epidemia;
}

Epidemia *criar_pequenos_mundos(int num_vertices, int k, float probabilidade) {
    Epidemia* epidemia = (Epidemia*)malloc(sizeof(Epidemia));
    if(epidemia == NULL){
        perror("Erro ao alocar epidemia");
        exit(EXIT_FAILURE);
    }

    Graph **grafo = (Graph**)malloc(num_vertices * sizeof(Graph *));
    if (!grafo) {
        free(epidemia);
        perror("Erro ao alocar grafo");
        exit(EXIT_FAILURE);
    }

    // Inicializa os nós
    for (int i = 0; i < num_vertices; i++) {
        grafo[i] = (Graph*)malloc(sizeof(Graph));
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

    epidemia->grafo = grafo;
    epidemia->curados = 0;
    epidemia->doentes = 0;
    epidemia->infectados = 0;
    epidemia->mortos = 0;
    epidemia->contaminantes = 0;

    return epidemia;
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
    
    for (int i = 0; i < num_vertices; i++) {
        // printf("%d\n", grafo[i]->estado);
        if(grafo[i] == NULL){
            printf("nulo\n");
            printf("%d\n", i);
            continue;
        }

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

void atualizar_estados(Epidemia *epidemia, int num_vertices) {
    Graph **grafo = epidemia->grafo;

    for (int i = 0; i < num_vertices; i++) {
        Graph *nodo = grafo[i];

        switch (nodo->estado) {
            case INFECTADO:
                nodo->dias_infectado++;
                if (nodo->dias_infectado >= DIAS_PARA_CONTAMINAR) {
                    nodo->estado = CONTAMINANTE;
                    epidemia->contaminantes += 1;
                    epidemia->infectados -= 1;
                }
                break;

            case CONTAMINANTE:
                if ((float)rand() / RAND_MAX < TAXA_RECUPERACAO) {
                    nodo->estado = CURADO;
                    epidemia->curados += 1;
                    epidemia->contaminantes -= 1;
                } else if ((float)rand() / RAND_MAX < TAXA_MORTALIDADE) {
                    nodo->estado = MORTO;
                    epidemia->mortos += 1;
                    epidemia->contaminantes -= 1;
                }
                break;

            case DOENTE:
                if ((float)rand() / RAND_MAX < TAXA_RECUPERACAO) {
                    nodo->estado = CURADO;
                    epidemia->curados += 1;
                    epidemia->doentes -= 1;
                } else if ((float)rand() / RAND_MAX < TAXA_MORTALIDADE) {
                    nodo->estado = MORTO;
                    epidemia->mortos += 1;
                    epidemia->doentes -= 1;
                }
                break;

            default:
                break;
        }
    }
}

void propagar_infeccao(Epidemia *epidemia, int num_vertices) {
    Graph **grafo = epidemia->grafo;

    for (int i = 0; i < num_vertices; i++) {
        Graph *nodo = grafo[i];

        if (nodo->estado == CONTAMINANTE) {
            for (int j = 0; j < nodo->num_amigos; j++) {
                Graph *vizinho = nodo->conexoes_de_amigos[j];
                if (vizinho->estado == SAUDAVEL && (float)rand() / RAND_MAX < TAXA_TRANSMISSAO) {
                    vizinho->estado = INFECTADO;
                    epidemia->infectados += 1;
                }
            }
        }
    }
}

void simular_epidemia(Epidemia *epidemia, int num_vertices, int num_iteracoes) {
    Graph **grafo = epidemia->grafo;

    // Infecta o primeiro nó para iniciar a epidemia
    grafo[0]->estado = INFECTADO;
    epidemia->infectados = 1;

    for(int i = 0; i < num_vertices; i++){
        if(grafo[i] == NULL){
            printf("É NULO, %d\n", i);
        }
    }

    for (int iteracao = 0; iteracao < num_iteracoes; iteracao++) {
        // Atualiza os estados e propaga a infecção
        atualizar_estados(epidemia, num_vertices);
        propagar_infeccao(epidemia, num_vertices);
        gravar_informacoes(iteracao, num_vertices, epidemia);
    }
}

void gravar_informacoes(int iteracao, int num_vertices, Epidemia* epidemia)
{
    static char pasta_dot_simulacao[100], pasta_svg_simulacao[100], pasta_plot_simulacao[100];

    if(iteracao == 0){
        char* dateTimeString = getDateAndTimeString();

        snprintf(pasta_plot_simulacao, 100, "plot/simulacao_%s", dateTimeString);
        mkdir(pasta_plot_simulacao, 0700);

        snprintf(pasta_dot_simulacao, 100, "output_dot/simulacao_%s", dateTimeString);
        mkdir(pasta_dot_simulacao, 0700);

        snprintf(pasta_svg_simulacao, 100, "output_svg/simulacao_%s", dateTimeString);
        mkdir(pasta_svg_simulacao, 0700);

        free(dateTimeString);
    }

    Graph **grafo = epidemia->grafo;

    char nome_arquivo_dot[200], command[400], nome_arquivo_plot[200];

    snprintf(nome_arquivo_dot, 200, "%s/simulacao_iteracao_%d.dot", pasta_dot_simulacao, iteracao);
    snprintf(command, 400, "dot %s -Tsvg > %s/%.*s.svg", nome_arquivo_dot, pasta_svg_simulacao, (int)(strlen(&nome_arquivo_dot[48]) - 4), &(nome_arquivo_dot[48]));
    // printf("%s\n %s\n", nome_arquivo_dot, command);
    gerar_graphviz(grafo, num_vertices, nome_arquivo_dot);
    system(command);

    // Plota as informações
    snprintf(nome_arquivo_plot, 200, "%s/infectados.txt", pasta_plot_simulacao);
    FILE* fp = fopen(nome_arquivo_plot, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->infectados);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot, 200, "%s/contaminantes.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->contaminantes);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot, 200, "%s/doentes.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->doentes);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot, 200, "%s/curados.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->curados);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot, 200, "%s/mortos.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->mortos);
    }
    fclose(fp);
}

char* getDateAndTimeString()
{
    time_t seconds;
    struct tm *dateAndTime;
    char *dateBuffer, *millisseconds;

    dateBuffer = (char*)malloc(27*sizeof(char));
    memset(dateBuffer, '\0', 27);

    if(dateBuffer == NULL){
        printf("Erro ao alocar string de data e hora\n");
        return NULL;
    }

    time(&seconds);
    dateAndTime = localtime(&seconds);

    if(dateAndTime->tm_mday < 10)
    {
        dateBuffer[0] = '0';
        sprintf(dateBuffer+1, "%d", dateAndTime->tm_mday);
    }
    else{
        sprintf(dateBuffer, "%d", dateAndTime->tm_mday);
    }

    dateBuffer[2] = '-';

    if(dateAndTime->tm_mon + 1 < 10 )
    {
        dateBuffer[3] = '0';
        sprintf(dateBuffer+4, "%d", dateAndTime->tm_mon + 1);
    }
    else{
        sprintf(dateBuffer+3, "%d", dateAndTime->tm_mon + 1);
    }

    dateBuffer[5] = '-';

    sprintf(dateBuffer+6, "%d", dateAndTime->tm_year + 1900);

    dateBuffer[10] = '_'; dateBuffer[11] = -61; dateBuffer[12] = -96; 
    dateBuffer[13] = 's'; dateBuffer[14] = '_'; 

    if(dateAndTime->tm_hour < 10)
    {
        dateBuffer[15] = '0';
        sprintf(dateBuffer+16, "%d", dateAndTime->tm_hour);
    }
    else{
        sprintf(dateBuffer+15, "%d", dateAndTime->tm_hour);
    }

    dateBuffer[17] = ':';

    if(dateAndTime->tm_min < 10)
    {
        dateBuffer[18] = '0';
        sprintf(dateBuffer+19, "%d", dateAndTime->tm_min);
    }
    else{
        sprintf(dateBuffer+18, "%d", dateAndTime->tm_min);
    }

    dateBuffer[20] = ':';

    if(dateAndTime->tm_sec < 10)
    {
        dateBuffer[21] = '0';
        sprintf(dateBuffer+22, "%d", dateAndTime->tm_sec);
    }
    else{
        sprintf(dateBuffer+21, "%d", dateAndTime->tm_sec);
    }

    millisseconds = getMillisseconds();

    if(millisseconds == NULL){
        printf("Erro ao alocar string de data e hora\n");
        return NULL;
    }

    dateBuffer[23] = ':';
    dateBuffer[24] = millisseconds[0];
    dateBuffer[25] = millisseconds[1];
    dateBuffer[26] = '\0';

    free(millisseconds);

    return dateBuffer;
}

char* getMillisseconds()
{
    char *result, *aux;
    int length, index;
    float value;

    value = (float)clock() / CLOCKS_PER_SEC;
    length = snprintf(NULL, 0, "%f", value);

    aux = (char*)malloc((length+1)*sizeof(char));

    if(aux == NULL){
        return NULL;
    }

    sprintf(aux, "%f", value);

    result = (char*)malloc(2*sizeof(char));

    if(result == NULL){
        free(aux);
        return NULL;
    }

    index = 0;
    while(aux[index] != '.') index++;

    result[0] = aux[index + 1];
    result[1] = aux[index + 2];

    free(aux);

    return result;
}