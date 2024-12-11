#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include "grafo.h"

void mostrar_resultados(int num_repeticoes);

// para pequenos mundos entre 0 e 0.3 está bom
// para aleatório a partir de 0.3

int main() {
    srand(time(NULL));
    int num_vertices = 20, num_repeticoes = 10;
    int k = 2;
    float probabilidade = REWIRE_PROBABILITY;

    Graph **grafo = criar_pequenos_mundos(num_vertices, k, probabilidade);

    simular_epidemia(grafo, num_vertices, num_repeticoes);

    mostrar_resultados(num_repeticoes);

    // Libera a memória
    for (int i = 0; i < num_vertices; i++) {
        free(grafo[i]->conexoes_de_amigos);
        free(grafo[i]->usuario);
        free(grafo[i]);
    }
    free(grafo);

    return 0;
}

void mostrar_resultados(int num_repeticoes)
{
    for(int i = 0; i < num_repeticoes; i++)
    {
        char command[200];
        pid_t pid = fork();
        snprintf(command, 200, "output_svg/epidemia_iteracao_%d.svg", i);

        if(pid == 0){
            execlp("xdg-open", "xdg-open", command, NULL);
        }

        while(getchar() != '\n');
    }
}