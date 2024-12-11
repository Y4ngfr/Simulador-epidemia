#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define NUM_USUARIOS_MAX 6
#define NUM_AMIGOS_PERMITIDOS NUM_USUARIOS_MAX
#define MAX_STRING 255
#define SIZE (NUM_USUARIOS_MAX * NUM_AMIGOS_PERMITIDOS)

typedef struct usuario
{
    unsigned int id;
    char nome[MAX_STRING];
} Usuario;

typedef struct graph_adjacencia
{
    struct nodo *amigo;
    int relevancia;
    struct aresta *posicao_aresta;
} GraphAdjacencia;

typedef struct nodo
{
    GraphAdjacencia **conexoes_de_amigos; // Lé-se isso aqui como um vetor de ponteiros (dá em uma matriz no final)
    Usuario *usuario;
    int num_amigos;
} Graph;

typedef struct aresta
{
    Graph *pessoa_origem;
    Graph *pessoa_destino;
    int relevancia;
} Aresta; 

typedef struct
{
    Aresta *arestas;
    int num_arestas;
} ListOfAresta;

typedef struct s_epidemia
{
    Graph **pessoas;
    ListOfAresta list_arestas;
    int num_usuarios;
    float densidade;
} SEpidemia;


typedef struct conexao {
    int *vizinhos;
    int *posicao;
    int tamanho_conexoes;
} Conexao;


typedef struct min_heap {
    int *vertices;
    int *indices;       
    int tamanho_heap;     
} MinHeap;

void swap(Aresta *arr, int i, int j) {
    Aresta aux = arr[i];
    arr[i] = arr[j];
    arr[j] = aux;
}

void minPercolate(MinHeap *heap, int i) {
    int index = i, left = 2 * i + 1, right = 2 * i + 2;
    
    if (left < heap->tamanho_heap && heap->indices[left] < heap->indices[index])
        index = left;
    if (right < heap->tamanho_heap && heap->indices[right] < heap->indices[index])
        index = right;

    if (index != i) {
        int aux = heap->vertices[i];
        heap->vertices[i] = heap->vertices[index];
        heap->vertices[index] = aux;

        int aux_indice = heap->indices[i];
        heap->indices[i] = heap->indices[index];
        heap->indices[index] = aux_indice;

        minPercolate(heap, index);
    }
}

void cria_minHeap(MinHeap *heap) {
    for (int i = heap->tamanho_heap / 2 - 1; i >= 0; i--) {
        minPercolate(heap, i);
    }
}


int extrai_minimo(MinHeap *heap) {
    if (heap->tamanho_heap == 0) return -1; 

    int minVertex = heap->vertices[0];
    
    heap->vertices[0] = heap->vertices[heap->tamanho_heap - 1];
    heap->indices[0] = heap->indices[heap->tamanho_heap - 1];

    heap->tamanho_heap--;
    minPercolate(heap, 0);

    return minVertex;
}


Conexao *conexao_init(int tamanho) {
    Conexao *conexoes = (Conexao *)malloc(sizeof(Conexao));
    conexoes->vizinhos = (int *)malloc(sizeof(int) * tamanho);
    conexoes->posicao = (int *)malloc(sizeof(int) * tamanho);
    conexoes->tamanho_conexoes = tamanho;
    for (int i = 0; i < tamanho; ++i) {
        conexoes->vizinhos[i] = i;
        conexoes->posicao[i] = 0;
    }
    return conexoes;
}



void gerar_nome(char *nome)
{
    const char *nomes[] = {"Alice", "Bob", "Carlos", "Daniela", "Eva", "Felipe", "Gustavo", "Helena", "Igor", "Juliana"};
    int index = rand() % 10;
    strcpy(nome, nomes[index]);
}

GraphAdjacencia **add_espaco_livre_conexoes_amigos(Graph *usuario, GraphAdjacencia *nova_adjacencia, Aresta *arestas)
{
    for (int i = 0; i < NUM_AMIGOS_PERMITIDOS; ++i)
    {
        if (usuario->conexoes_de_amigos[i] == NULL)
        {
            usuario->conexoes_de_amigos[i] = nova_adjacencia;

            for (int j = 0; j < SIZE; ++j)
            {
                if (arestas[j].pessoa_origem == NULL)
                {
                    arestas[j].pessoa_origem = usuario;
                    arestas[j].pessoa_destino = nova_adjacencia->amigo;
                    arestas[j].relevancia = nova_adjacencia->relevancia;
                    nova_adjacencia->posicao_aresta = &(arestas[j]);
                    return usuario->conexoes_de_amigos;
                }
            }
            return NULL;
        }
    }

    return NULL;
}

SEpidemia add_amizade(SEpidemia friend_f, int id_usuario_solicitante, int id_usuario_adicionado, int relevancia)
{
    Graph *usuario_solicitante = NULL, *usuario_adicionado = NULL;
    int i;
    for (i = 0; i < NUM_USUARIOS_MAX; ++i)
    {
        if (friend_f.pessoas[i]->usuario == NULL)
            break;

        if (friend_f.pessoas[i]->usuario->id == id_usuario_solicitante)
            usuario_solicitante = friend_f.pessoas[i];

        if (friend_f.pessoas[i]->usuario->id == id_usuario_adicionado)
            usuario_adicionado = friend_f.pessoas[i];
    }

    if ((usuario_solicitante != NULL) && (usuario_adicionado != NULL))
    {
        if (usuario_solicitante->num_amigos == NUM_AMIGOS_PERMITIDOS)
        {
            return friend_f;
        }

        for (i = 0; i < usuario_solicitante->num_amigos; ++i)
        {
            if (usuario_solicitante->conexoes_de_amigos[i]->amigo->usuario->id == id_usuario_adicionado)
            {
                return friend_f;
            }
        }

        GraphAdjacencia *nova_adjacencia;
        nova_adjacencia = (GraphAdjacencia *)malloc(sizeof(GraphAdjacencia));
        nova_adjacencia->amigo = usuario_adicionado;
        nova_adjacencia->relevancia = relevancia;

        if (usuario_solicitante->num_amigos >= NUM_AMIGOS_PERMITIDOS)
        {
           
            return friend_f;
        }
        int posicao_ar = friend_f.list_arestas.num_arestas;

        friend_f.list_arestas.num_arestas++;

        friend_f.list_arestas.arestas[posicao_ar].pessoa_origem = usuario_solicitante;
        friend_f.list_arestas.arestas[posicao_ar].pessoa_destino = usuario_adicionado;
        friend_f.list_arestas.arestas[posicao_ar].relevancia = relevancia;

        nova_adjacencia->posicao_aresta = &friend_f.list_arestas.arestas[posicao_ar];

        usuario_solicitante->conexoes_de_amigos[usuario_solicitante->num_amigos] = nova_adjacencia;
        usuario_solicitante->num_amigos++;
        friend_f.pessoas[id_usuario_solicitante] = usuario_solicitante;
    }

    return friend_f;
}

void gerar_usuarios(Usuario *usuarios, int num_usuarios)
{
    for (int i = 0; i < num_usuarios; i++)
    {
        usuarios[i].id = i;
        gerar_nome(usuarios[i].nome);
    }
}

float calcula_densidade_grafo(SEpidemia friend_f)
{
    int numero_vertices = friend_f.num_usuarios;
    if (numero_vertices <= 1)
        return 0.0;

    int numero_arestas = friend_f.list_arestas.num_arestas;
    return (float)(numero_arestas) / (numero_vertices * (numero_vertices - 1));
}

void calcula_grau_entrada(SEpidemia friend_f, int *grau_entrada)
{
    int i;
    for (i = 0; i < friend_f.num_usuarios; ++i)
    {
        grau_entrada[i] = friend_f.pessoas[i]->num_amigos;
    }
}


void preenche_pilha(int indice_vertice, bool *visitado, SEpidemia friend_f, int *pilha, int *topo)
{
    visitado[indice_vertice] = true;

    for (int i = 0; i < friend_f.pessoas[indice_vertice]->num_amigos; i++)
    {
        int amigo_id = friend_f.pessoas[indice_vertice]->conexoes_de_amigos[i]->amigo->usuario->id;
        if (!visitado[amigo_id])
        {
            preenche_pilha(amigo_id, visitado, friend_f, pilha, topo);
        }
    }

    pilha[(*topo)++] = indice_vertice;
}



void start_graphviz()
{
    printf("digraph G {");
}

void print_graphviz(SEpidemia friend_f);

void print_graph_by_id(SEpidemia friend_f, int *ids, char *nome_grafo, bool conexao);

void end_graphviz()
{
    printf("}");
}

int main() {
    ListOfAresta list_arestas;
    SEpidemia friend_f;
    Aresta *arestas = (Aresta *)malloc(sizeof(Aresta) * SIZE);
    srand(time(NULL));

    friend_f.pessoas = (Graph **)malloc(sizeof(Graph *) * NUM_USUARIOS_MAX);

    for (int i = 0; i < NUM_USUARIOS_MAX; i++) {
        friend_f.pessoas[i] = (Graph *)malloc(sizeof(Graph));
        friend_f.pessoas[i]->conexoes_de_amigos = (GraphAdjacencia **)malloc(sizeof(GraphAdjacencia *) * NUM_AMIGOS_PERMITIDOS);

        for (int j = 0; j < NUM_AMIGOS_PERMITIDOS; ++j) {
            friend_f.pessoas[i]->conexoes_de_amigos[j] = NULL;
        }

        friend_f.pessoas[i]->usuario = (Usuario *)malloc(sizeof(Usuario));
        friend_f.pessoas[i]->usuario->id = i;
        friend_f.pessoas[i]->num_amigos = 0;
        gerar_nome(friend_f.pessoas[i]->usuario->nome);
    }

    friend_f.num_usuarios = NUM_USUARIOS_MAX;
    list_arestas.arestas = arestas;
    list_arestas.num_arestas = 0;

    friend_f.list_arestas = list_arestas;

    // for (int i = 0; i < friend_f.num_usuarios; i++)
    // {
    //     int relevancia = rand() % 100 + 1;
    //     for (int j = 0; j < NUM_AMIGOS_PERMITIDOS; ++j)
    //     {
    //         int rn = rand() % friend_f.num_usuarios;
    //         if (rn == i)
    //             continue;
    //         friend_f = add_amizade(friend_f, i, rn, relevancia);
    //     }
    // }

    friend_f = add_amizade(friend_f, 0, 1, 4);
    friend_f = add_amizade(friend_f, 0, 2, 3);
    friend_f = add_amizade(friend_f, 1, 2, 1);
    friend_f = add_amizade(friend_f, 1, 3, 2);
    friend_f = add_amizade(friend_f, 2, 3, 4);
    friend_f = add_amizade(friend_f, 2, 4, 6);
    friend_f = add_amizade(friend_f, 2, 5, 5);
    friend_f = add_amizade(friend_f, 3, 4, 5);
    
    friend_f.densidade = calcula_densidade_grafo(friend_f);

    start_graphviz();
    {
        print_graphviz(friend_f);

        int *kruskal_r = kruskal(friend_f, 0);
        if(kruskal_r != NULL) {
            print_graph_by_id(friend_f, kruskal_r, "kruskal", true);
        }
        free(kruskal_r); 

        int *prim_r = prim(friend_f, 0);

        if(prim_r != NULL) {
            print_graph_by_id(friend_f, prim_r, "prim", true);
        }

        free(prim_r); 
        

        int *khan_r = khan(friend_f);

        if(khan_r != NULL) {
            print_graph_by_id(friend_f, khan_r, "khan", false);
        }

        free(khan_r); 


        int *dfs_r = dfs(friend_f);

        if(dfs_r != NULL) {
            print_graph_by_id(friend_f, dfs_r, "dfs", false);
        }

        free(dfs_r); 

    }
    end_graphviz();
    free(arestas);
    free(friend_f.pessoas); 

    return 0;
}

void print_graphviz(SEpidemia friend_f)
{
    int i, j, num_amigos;
    printf("subgraph cluster_0 {");

    for (i = 0; i < friend_f.num_usuarios; ++i)
    {
        Graph *nodo = friend_f.pessoas[i];
        num_amigos = friend_f.pessoas[i]->num_amigos;
        for (j = 0; j < num_amigos; ++j) {
            printf("%s_%d->", nodo->usuario->nome, nodo->usuario->id);
            printf("%s_%d [label=%d];", nodo->conexoes_de_amigos[j]->amigo->usuario->nome, nodo->conexoes_de_amigos[j]->amigo->usuario->id, nodo->conexoes_de_amigos[j]->relevancia);
        }
    }
    printf("\nlabel = \"Grafo completo/Grafo %s - densidade = %lf\";", friend_f.densidade > 0.5 ? "Denso" : "Esparso", friend_f.densidade);
    printf("}");
}


void print_graph_by_id(SEpidemia friend_f, int *ids, char *nome_grafo, bool conexao) {
    int i;
    printf("subgraph cluster_%s {", nome_grafo);
    for(i = 0; i < friend_f.num_usuarios; ++i) {
        if(ids[i] == -1) continue;
        if(conexao) 
            printf("%s_%d_%s->", friend_f.pessoas[i]->usuario->nome, friend_f.pessoas[i]->usuario->id, nome_grafo);
        printf("%s_%d_%s", friend_f.pessoas[ids[i]]->usuario->nome, friend_f.pessoas[ids[i]]->usuario->id, nome_grafo);
        printf(";");
    }
    printf("\nlabel=\"%s\"}", nome_grafo);
}
