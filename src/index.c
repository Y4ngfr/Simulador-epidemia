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
    Graph *usuario_origem;
    Graph *usuario_destino;
    int relevancia;
} Aresta; 

typedef struct
{
    Aresta *arestas;
    int num_arestas;
} ListOfAresta;

typedef struct friend_face
{
    Graph **usuarios_sistema;
    ListOfAresta list_arestas;
    int num_usuarios;
    float densidade;
} FriendFace;


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


void decrementa_indice(MinHeap *heap, int vertice, int novo_indice) {
    int i;
    for (i = 0; i < heap->tamanho_heap; ++i) {
        if (heap->vertices[i] == vertice)
            break;
    }

    heap->indices[i] = novo_indice;

    while (i > 0 && heap->indices[i] < heap->indices[(i - 1) / 2]) {
        int aux = heap->vertices[i];
        heap->vertices[i] = heap->vertices[(i - 1) / 2];
        heap->vertices[(i - 1) / 2] = aux;

        int aux_indice = heap->indices[i];
        heap->indices[i] = heap->indices[(i - 1) / 2];
        heap->indices[(i - 1) / 2] = aux_indice;

        i = (i - 1) / 2;
    }
}

int *prim(FriendFace friend_f, int origem) {
    int i, j;
    int *parent = (int *)malloc(sizeof(int) * friend_f.num_usuarios);
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));

    int num_usuarios = friend_f.num_usuarios;

    minHeap->vertices = (int *)malloc(sizeof(int) * num_usuarios);
    minHeap->indices = (int *)malloc(sizeof(int) * num_usuarios);
    minHeap->tamanho_heap = num_usuarios;

    for (i = 0; i < num_usuarios; ++i) {
        minHeap->vertices[i] = i;
        minHeap->indices[i] = INT_MAX;
        parent[i] = -1;  
    }

    minHeap->indices[origem] = 0; 
    
    cria_minHeap(minHeap);

    while (minHeap->tamanho_heap > 0) {
        int u = extrai_minimo(minHeap);
        
        for (i = 0; i < friend_f.usuarios_sistema[u]->num_amigos; ++i) {
            GraphAdjacencia *conexao_analisada = friend_f.usuarios_sistema[u]->conexoes_de_amigos[i];
            int vertice_id = conexao_analisada->amigo->usuario->id;
            int relevancia = conexao_analisada->relevancia;
            
            if (minHeap->indices[vertice_id] > relevancia) {
                decrementa_indice(minHeap, vertice_id, relevancia);
                parent[vertice_id] = u;
            }
        }
    }

    free(minHeap->vertices);
    free(minHeap->indices);
    free(minHeap);

    return parent;
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


int busca_conexoes(Conexao *conexoes, int indice) {
    if (conexoes->vizinhos[indice] != indice) {
        conexoes->vizinhos[indice] = busca_conexoes(conexoes, conexoes->vizinhos[indice]);
    }
    return conexoes->vizinhos[indice];
}

void faz_uniao_conexao(Conexao *conexao, int x, int y) {
    int raizA = busca_conexoes(conexao, x);
    int raizB = busca_conexoes(conexao, y);
    
    if (raizA != raizB) {
        if (conexao->posicao[raizA] > conexao->posicao[raizB]) {
            conexao->vizinhos[raizB] = raizA;
        } else if (conexao->posicao[raizA] < conexao->posicao[raizB]) {
            conexao->vizinhos[raizA] = raizB;
        } else {
            conexao->vizinhos[raizB] = raizA;
            conexao->posicao[raizA]++;
        }
    }
}
int comparacao(const void *a, const void *b) {
    Aresta *a_a = (Aresta *)a;
    Aresta *a_b = (Aresta *)b;
    return a_a->relevancia - a_b->relevancia;
}

int *kruskal(FriendFace friend_f, int origem) {
    int i, *arvore_geradora;
    int num_usuarios = friend_f.num_usuarios;

    Conexao *conexao = conexao_init(num_usuarios);
    arvore_geradora = (int *)malloc(sizeof(int) * num_usuarios);

    for (i = 0; i < num_usuarios; ++i) {
        arvore_geradora[i] = -1;  
    }

    Aresta *arestas = (Aresta *)malloc(sizeof(Aresta) * SIZE); 
    int num_arestas = 0;

    for (i = 0; i < num_usuarios; ++i) {
        for (int j = 0; j < friend_f.usuarios_sistema[i]->num_amigos; ++j) {
            GraphAdjacencia *conexao_analisada = friend_f.usuarios_sistema[i]->conexoes_de_amigos[j];
            int origem = i;
            int destino = conexao_analisada->amigo->usuario->id;
            int relevancia = conexao_analisada->relevancia;

            arestas[num_arestas].usuario_origem = friend_f.usuarios_sistema[origem];
            arestas[num_arestas].usuario_destino = friend_f.usuarios_sistema[destino];
            arestas[num_arestas].relevancia = relevancia;
            num_arestas++;
        }
    }

    qsort(arestas, num_arestas, sizeof(Aresta), comparacao);

    for (i = 0; i < num_arestas; ++i) {
        int origem = arestas[i].usuario_origem->usuario->id;
        int destino = arestas[i].usuario_destino->usuario->id;

        if (busca_conexoes(conexao, origem) != busca_conexoes(conexao, destino)) {
            faz_uniao_conexao(conexao, origem, destino);
            arvore_geradora[origem] = destino; 
        }
    }

    free(arestas);
    free(conexao->vizinhos);
    free(conexao->posicao);
    free(conexao);

    return arvore_geradora;
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
                if (arestas[j].usuario_origem == NULL)
                {
                    arestas[j].usuario_origem = usuario;
                    arestas[j].usuario_destino = nova_adjacencia->amigo;
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

FriendFace add_amizade(FriendFace friend_f, int id_usuario_solicitante, int id_usuario_adicionado, int relevancia)
{
    Graph *usuario_solicitante = NULL, *usuario_adicionado = NULL;
    int i;
    for (i = 0; i < NUM_USUARIOS_MAX; ++i)
    {
        if (friend_f.usuarios_sistema[i]->usuario == NULL)
            break;

        if (friend_f.usuarios_sistema[i]->usuario->id == id_usuario_solicitante)
            usuario_solicitante = friend_f.usuarios_sistema[i];

        if (friend_f.usuarios_sistema[i]->usuario->id == id_usuario_adicionado)
            usuario_adicionado = friend_f.usuarios_sistema[i];
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

        friend_f.list_arestas.arestas[posicao_ar].usuario_origem = usuario_solicitante;
        friend_f.list_arestas.arestas[posicao_ar].usuario_destino = usuario_adicionado;
        friend_f.list_arestas.arestas[posicao_ar].relevancia = relevancia;

        nova_adjacencia->posicao_aresta = &friend_f.list_arestas.arestas[posicao_ar];

        usuario_solicitante->conexoes_de_amigos[usuario_solicitante->num_amigos] = nova_adjacencia;
        usuario_solicitante->num_amigos++;
        friend_f.usuarios_sistema[id_usuario_solicitante] = usuario_solicitante;
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

float calcula_densidade_grafo(FriendFace friend_f)
{
    int numero_vertices = friend_f.num_usuarios;
    if (numero_vertices <= 1)
        return 0.0;

    int numero_arestas = friend_f.list_arestas.num_arestas;
    return (float)(numero_arestas) / (numero_vertices * (numero_vertices - 1));
}

void calcula_grau_entrada(FriendFace friend_f, int *grau_entrada)
{
    int i;
    for (i = 0; i < friend_f.num_usuarios; ++i)
    {
        grau_entrada[i] = friend_f.usuarios_sistema[i]->num_amigos;
    }
}

int *khan(FriendFace friend_f)
{
    int *grau_entrada, *ordenacao, indice = 0;
    int *fila, inicio_fila = 0, final_fila = 0;

    grau_entrada = (int *)malloc(sizeof(int) * friend_f.num_usuarios);
    ordenacao = (int *)malloc(sizeof(int) * friend_f.num_usuarios);
    fila = (int *)malloc(sizeof(int) * friend_f.num_usuarios);

    for (int i = 0; i < friend_f.num_usuarios; i++) {
        grau_entrada[i] = 0;
    }

    for (int i = 0; i < friend_f.num_usuarios; i++) {
        for (int j = 0; j < friend_f.usuarios_sistema[i]->num_amigos; j++) {
            int amigo_id = friend_f.usuarios_sistema[i]->conexoes_de_amigos[j]->amigo->usuario->id;
            grau_entrada[amigo_id]++;
        }
    }

    for (int i = 0; i < friend_f.num_usuarios; i++) {
        if (grau_entrada[i] == 0) {
            fila[final_fila++] = i;
        }
    }

    while (inicio_fila < final_fila) {
        int atual = fila[inicio_fila++];
        ordenacao[indice++] = atual;

        for (int j = 0; j < friend_f.usuarios_sistema[atual]->num_amigos; j++) {
            int amigo_id = friend_f.usuarios_sistema[atual]->conexoes_de_amigos[j]->amigo->usuario->id;
            grau_entrada[amigo_id]--;
            if (grau_entrada[amigo_id] == 0) {
                fila[final_fila++] = amigo_id;
            }
        }
    }

    if (indice != friend_f.num_usuarios) {
        printf("\"O grafo contém ciclos e não pode ser ordenado topologicamente. (khan)\"");
        free(grau_entrada);
        free(ordenacao);
        free(fila);
        return NULL;
    }

    free(grau_entrada);
    free(fila);
    return ordenacao;
}


void preenche_pilha(int indice_vertice, bool *visitado, FriendFace friend_f, int *pilha, int *topo)
{
    visitado[indice_vertice] = true;

    for (int i = 0; i < friend_f.usuarios_sistema[indice_vertice]->num_amigos; i++)
    {
        int amigo_id = friend_f.usuarios_sistema[indice_vertice]->conexoes_de_amigos[i]->amigo->usuario->id;
        if (!visitado[amigo_id])
        {
            preenche_pilha(amigo_id, visitado, friend_f, pilha, topo);
        }
    }

    pilha[(*topo)++] = indice_vertice;
}

int *dfs(FriendFace friend_f)
{
    bool *visitado;
    int *pilha;
    int topo = 0;

    visitado = (bool *)malloc(sizeof(bool) * friend_f.num_usuarios);
    pilha = (int *)malloc(sizeof(int) * friend_f.num_usuarios);

    memset(visitado, 0, sizeof(bool) * friend_f.num_usuarios);

    for (int i = 0; i < friend_f.num_usuarios; i++)
    {
        if (!visitado[i])
        {
            preenche_pilha(i, visitado, friend_f, pilha, &topo);
        }
    }

    return pilha;
}

void start_graphviz()
{
    printf("digraph G {");
}

void print_graphviz(FriendFace friend_f);

void print_graph_by_id(FriendFace friend_f, int *ids, char *nome_grafo, bool conexao);

void end_graphviz()
{
    printf("}");
}

int main() {
    ListOfAresta list_arestas;
    FriendFace friend_f;
    Aresta *arestas = (Aresta *)malloc(sizeof(Aresta) * SIZE);
    srand(time(NULL));

    friend_f.usuarios_sistema = (Graph **)malloc(sizeof(Graph *) * NUM_USUARIOS_MAX);

    for (int i = 0; i < NUM_USUARIOS_MAX; i++) {
        friend_f.usuarios_sistema[i] = (Graph *)malloc(sizeof(Graph));
        friend_f.usuarios_sistema[i]->conexoes_de_amigos = (GraphAdjacencia **)malloc(sizeof(GraphAdjacencia *) * NUM_AMIGOS_PERMITIDOS);

        for (int j = 0; j < NUM_AMIGOS_PERMITIDOS; ++j) {
            friend_f.usuarios_sistema[i]->conexoes_de_amigos[j] = NULL;
        }

        friend_f.usuarios_sistema[i]->usuario = (Usuario *)malloc(sizeof(Usuario));
        friend_f.usuarios_sistema[i]->usuario->id = i;
        friend_f.usuarios_sistema[i]->num_amigos = 0;
        gerar_nome(friend_f.usuarios_sistema[i]->usuario->nome);
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
    free(friend_f.usuarios_sistema); 

    return 0;
}

void print_graphviz(FriendFace friend_f)
{
    int i, j, num_amigos;
    printf("subgraph cluster_0 {");

    for (i = 0; i < friend_f.num_usuarios; ++i)
    {
        Graph *nodo = friend_f.usuarios_sistema[i];
        num_amigos = friend_f.usuarios_sistema[i]->num_amigos;
        for (j = 0; j < num_amigos; ++j) {
            printf("%s_%d->", nodo->usuario->nome, nodo->usuario->id);
            printf("%s_%d [label=%d];", nodo->conexoes_de_amigos[j]->amigo->usuario->nome, nodo->conexoes_de_amigos[j]->amigo->usuario->id, nodo->conexoes_de_amigos[j]->relevancia);
        }
    }
    printf("\nlabel = \"Grafo completo/Grafo %s - densidade = %lf\";", friend_f.densidade > 0.5 ? "Denso" : "Esparso", friend_f.densidade);
    printf("}");
}


void print_graph_by_id(FriendFace friend_f, int *ids, char *nome_grafo, bool conexao) {
    int i;
    printf("subgraph cluster_%s {", nome_grafo);
    for(i = 0; i < friend_f.num_usuarios; ++i) {
        if(ids[i] == -1) continue;
        if(conexao) 
            printf("%s_%d_%s->", friend_f.usuarios_sistema[i]->usuario->nome, friend_f.usuarios_sistema[i]->usuario->id, nome_grafo);
        printf("%s_%d_%s", friend_f.usuarios_sistema[ids[i]]->usuario->nome, friend_f.usuarios_sistema[ids[i]]->usuario->id, nome_grafo);
        printf(";");
    }
    printf("\nlabel=\"%s\"}", nome_grafo);
}
