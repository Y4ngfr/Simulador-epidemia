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
    epidemia->atual_curados = 0;
    epidemia->atual_infectados = 0;
    epidemia->atual_mortos = 0;
    epidemia->atual_contaminantes = 0;

    epidemia->total_infectados = 0;
    epidemia->total_contaminantes = 0;

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
    epidemia->atual_curados = 0;
    epidemia->atual_infectados = 0;
    epidemia->atual_mortos = 0;
    epidemia->atual_contaminantes = 0;

    epidemia->total_infectados = 0;
    epidemia->total_contaminantes = 0;

    printf("teste criacao rede\n");

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
    epidemia->atual_curados = 0;
    epidemia->atual_infectados = 0;
    epidemia->atual_mortos = 0;
    epidemia->atual_contaminantes = 0;

    epidemia->total_infectados = 0;
    epidemia->total_contaminantes = 0;

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
        // {"red"},       // DOENTE
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
                    epidemia->atual_contaminantes += 1;
                    epidemia->atual_infectados -= 1;
                    epidemia->total_contaminantes += 1;
                }
                break;

            case CONTAMINANTE:
                if ((float)rand() / RAND_MAX < TAXA_RECUPERACAO) {
                    nodo->estado = CURADO;
                    epidemia->atual_curados += 1;
                    epidemia->atual_contaminantes -= 1;
                } else if ((float)rand() / RAND_MAX < TAXA_MORTALIDADE) {
                    nodo->estado = MORTO;
                    epidemia->atual_mortos += 1;
                    epidemia->atual_contaminantes -= 1;
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
                    epidemia->atual_infectados += 1;
                    epidemia->total_infectados += 1;
                }
            }
        }
    }
}

void simular_epidemia(Epidemia *epidemia, int num_vertices, int num_iteracoes, bool gerar_imagens_do_grafo) {
    Graph **grafo = epidemia->grafo;

    // Infecta o primeiro nó para iniciar a epidemia
    grafo[0]->estado = INFECTADO;
    epidemia->atual_infectados = 1;
    epidemia->total_infectados = 1;

    for (int iteracao = 0; iteracao < num_iteracoes; iteracao++) {
        // Atualiza os estados e propaga a infecção
        atualizar_estados(epidemia, num_vertices);
        propagar_infeccao(epidemia, num_vertices);
        gravar_informacoes(iteracao, num_iteracoes, num_vertices, epidemia, gerar_imagens_do_grafo);
    }
}

void gravar_informacoes(int iteracao, int num_iteracoes, int num_vertices, Epidemia* epidemia, bool gerar_imagens_do_grafo)
{
    static char pasta_dot_simulacao[100], pasta_svg_simulacao[100], pasta_plot_simulacao[100];
    static char pasta_graficos_simulacao[100], pasta_csv_simulacao[100];
    static int pico_infectados = 0, tempo_ate_pico;

    if(iteracao == 0){
        char* dateTimeString = getDateAndTimeString();

        snprintf(pasta_plot_simulacao, 100, "plot/simulacao_%s", dateTimeString);
        mkdir(pasta_plot_simulacao, 0700);

        snprintf(pasta_dot_simulacao, 100, "output_dot/simulacao_%s", dateTimeString);
        mkdir(pasta_dot_simulacao, 0700);

        snprintf(pasta_svg_simulacao, 100, "output_svg/simulacao_%s", dateTimeString);
        mkdir(pasta_svg_simulacao, 0700);

        snprintf(pasta_graficos_simulacao, 100, "graficos/simulacao_%s", dateTimeString);
        mkdir(pasta_graficos_simulacao, 0700);

        snprintf(pasta_csv_simulacao, 100, "csv_out/simulacao_%s", dateTimeString);
        mkdir(pasta_csv_simulacao, 0700);

        free(dateTimeString);
    }

    Graph **grafo = epidemia->grafo;

    char nome_arquivo_dot[200], command[400];
    char nome_arquivo_plot_infectados[200];
    char nome_arquivo_plot_contaminantes[200];
    char nome_arquivo_plot_curados[200];
    char nome_arquivo_plot_mortos[200];

    snprintf(nome_arquivo_dot, 200, "%s/simulacao_iteracao_%d.dot", pasta_dot_simulacao, iteracao);
    snprintf(command, 400, "dot %s -Tsvg > %s/%.*s.svg", nome_arquivo_dot, pasta_svg_simulacao, (int)(strlen(&nome_arquivo_dot[48]) - 4), &(nome_arquivo_dot[48]));

    if(gerar_imagens_do_grafo){
        gerar_graphviz(grafo, num_vertices, nome_arquivo_dot);
        system(command);
    }

    // Plota as informações
    snprintf(nome_arquivo_plot_infectados, 200, "%s/infectados.txt", pasta_plot_simulacao);
    FILE* fp = fopen(nome_arquivo_plot_infectados, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->atual_infectados);
    }
    fclose(fp);

    if(epidemia->atual_infectados >= pico_infectados){
        pico_infectados = epidemia->atual_infectados;
        tempo_ate_pico = iteracao;
    }

    snprintf(nome_arquivo_plot_contaminantes, 200, "%s/contaminantes.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot_contaminantes, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->atual_contaminantes);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot_curados, 200, "%s/curados.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot_curados, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->atual_curados);
    }
    fclose(fp);

    snprintf(nome_arquivo_plot_mortos, 200, "%s/mortos.txt", pasta_plot_simulacao);
    fp = fopen(nome_arquivo_plot_mortos, "a+");
    if(fp){
        fprintf(fp, "%d %d\n", iteracao, epidemia->atual_mortos);
    }
    fclose(fp);

    if(iteracao == num_iteracoes - 1){
        char nome_arquivo_gp[200], command[250];
        snprintf(nome_arquivo_gp, 200, "%s/temp_grafico.gp", pasta_graficos_simulacao);
        snprintf(command, 250, "gnuplot %s/temp_grafico.gp", pasta_graficos_simulacao);

        fp = fopen(nome_arquivo_gp, "w");
        fprintf(fp, "set title \"Grafico simulacao\"\nset xlabel \"Eixo X\"\nset ylabel \"Eixo Y\"\nset terminal png size 800,600\nset output \"%s/contaminantes.png\"\nplot \"%s\" using 1:2 with linespoints title \"Dados\"\n", pasta_graficos_simulacao, nome_arquivo_plot_contaminantes);
        fclose(fp);

        system(command);        

        fp = fopen(nome_arquivo_gp, "w");
        fprintf(fp, "set title \"Grafico simulacao\"\nset xlabel \"Eixo X\"\nset ylabel \"Eixo Y\"\nset terminal png size 800,600\nset output \"%s/curados.png\"\nplot \"%s\" using 1:2 with linespoints title \"Dados\"\n", pasta_graficos_simulacao, nome_arquivo_plot_curados);
        fclose(fp);

        system(command);        

        fp = fopen(nome_arquivo_gp, "w");
        fprintf(fp, "set title \"Grafico simulacao\"\nset xlabel \"Eixo X\"\nset ylabel \"Eixo Y\"\nset terminal png size 800,600\nset output \"%s/infectados.png\"\nplot \"%s\" using 1:2 with linespoints title \"Dados\"\n", pasta_graficos_simulacao, nome_arquivo_plot_infectados);
        fclose(fp);

        system(command);        

        fp = fopen(nome_arquivo_gp, "w");
        fprintf(fp, "set title \"Grafico simulacao\"\nset xlabel \"Eixo X\"\nset ylabel \"Eixo Y\"\nset terminal png size 800,600\nset output \"%s/mortos.png\"\nplot \"%s\" using 1:2 with linespoints title \"Dados\"\n", pasta_graficos_simulacao, nome_arquivo_plot_mortos);
        fclose(fp);
    
        system(command);        

        snprintf(command, 250, "rm %s/temp_grafico.gp", pasta_graficos_simulacao);
        system(command);


        gerar_csv(epidemia, pasta_csv_simulacao, pico_infectados, tempo_ate_pico, num_iteracoes, num_vertices, nome_arquivo_plot_infectados);
    }
}

void gerar_csv(Epidemia* epidemia, char* pasta_csv_simulacao, int pico_infectados, int tempo_ate_pico, int num_iteracoes, int num_vertices, char* nome_arquivo_plot_infectados)
{
    char arquivo_csv_simulacao[200], tempBuff[50];
    double media = 0, variancia = 0, desvio_padrao;
    FILE* fp;

    snprintf(arquivo_csv_simulacao, 200, "%s/tabela.csv", pasta_csv_simulacao);

    fp = fopen(nome_arquivo_plot_infectados, "r");

    while(!feof(fp)){
        fscanf(fp, "%s", tempBuff);
        fscanf(fp, "%s", tempBuff);
        
        media += (double)(atoi(tempBuff));
    }
    media /= (double)(num_iteracoes);
    printf("média: %f\n", media);

    fseek(fp, 0, SEEK_SET);

    while(!feof(fp)){
        fscanf(fp, "%s", tempBuff);
        fscanf(fp, "%s", tempBuff);
        
        variancia += pow((double)(atoi(tempBuff)) - media, 2);
    }

    variancia /= num_iteracoes;
    desvio_padrao = sqrt(variancia);

    printf("variancia: %f, desvio padrao: %f\n", variancia, desvio_padrao);

    fclose(fp);

    fp = fopen(arquivo_csv_simulacao, "w");

    fprintf(fp, "taxa_transmissao,taxa_recuperacao,taxa_mortalidade,num_vertices,num_repeticoes,total_infectados,total_contaminantes,total_mortos,pico_infectados,tempo_ate_pico,media_infeccao,desvio_padrao_infeccao,variancia_infeccao\n");
    fprintf(fp, "%.2f,%.2f,%.2f,%d,%d,%d,%d,%d,%d,%d,%.2f,%.2f,%.2f\n", TAXA_TRANSMISSAO, TAXA_RECUPERACAO, TAXA_MORTALIDADE, num_vertices, num_iteracoes, epidemia->total_infectados, epidemia->total_contaminantes, epidemia->atual_mortos, pico_infectados, tempo_ate_pico, media, desvio_padrao, variancia);

    fclose(fp);
}

char* getDateAndTimeString()
{
    time_t seconds;
    struct tm *dateAndTime;
    char *dateBuffer, *millisseconds;

    dateBuffer = (char*)malloc(26*sizeof(char));
    memset(dateBuffer, '\0', 26);

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

    dateBuffer[10] = '_'; dateBuffer[11] = '_'; dateBuffer[12] = 'a';
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