#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <time.h>
#include <signal.h>
#include "epidemia.h"

void mostrar_resultados(int num_repeticoes);

int main() {
    srand(time(NULL));
    int num_vertices = 10000, num_repeticoes = 100;
    int k = 4;
    float probabilidade = REWIRE_PROBABILITY;

    // Epidemia *epidemia = criar_pequenos_mundos(num_vertices, k, probabilidade);
    // Epidemia *epidemia = criar_rede_aleatoria(num_vertices, probabilidade);
    Epidemia *epidemia = criar_rede_livre_escala(num_vertices, k);

    simular_epidemia(epidemia, num_vertices, num_repeticoes, false);

    // Libera a memória
    for (int i = 0; i < num_vertices; i++) {
        free(epidemia->grafo[i]->conexoes_de_amigos);
        free(epidemia->grafo[i]->usuario);
        free(epidemia->grafo[i]);
    }
    free(epidemia->grafo);
    free(epidemia);

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