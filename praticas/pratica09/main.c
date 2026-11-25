#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

static GrafoMatriz* gerar_grafo_aleatorio(int n, int prob_percent) {
    GrafoMatriz *g = criar_grafo(n);
    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if ((rand() % 100) < prob_percent) {
                add_aresta(g, u, v);
            }
        }
    }
    return g;
}

int main(void) {
    srand(42);
    int tamanhos[] = {5, 8, 10, 12, 15};
    int num_testes = 5;

    printf("=== Experimento: Cobertura de Vertices (Backtracking vs Guloso) ===\n\n");
    printf("%-4s | %-10s | %-12s | %-10s | %-12s\n",
           "n", "Tam BT", "Tempo BT (s)", "Tam Guloso", "Tempo Gul (s)");
    printf("-----+------------+--------------+------------+--------------\n");

    for (int t = 0; t < num_testes; t++) {
        int n = tamanhos[t];
        GrafoMatriz *g = gerar_grafo_aleatorio(n, 45);

        int *sol_bt = (int*) malloc(n * sizeof(int));
        int *sol_gul = (int*) malloc(n * sizeof(int));

        clock_t ini_bt = clock();
        int tam_bt = cobertura_minima_bt(g, sol_bt);
        clock_t fim_bt = clock();
        double tempo_bt = (double)(fim_bt - ini_bt) / CLOCKS_PER_SEC;

        clock_t ini_gul = clock();
        int tam_gul = cobertura_gulosa(g, sol_gul);
        clock_t fim_gul = clock();
        double tempo_gul = (double)(fim_gul - ini_gul) / CLOCKS_PER_SEC;

        if (!eh_cobertura(g, sol_bt) || !eh_cobertura(g, sol_gul)) {
            printf("Erro: solucao invalida gerada para n = %d\n", n);
        }

        printf("%-4d | %-10d | %-12.6f | %-10d | %-12.6f\n",
               n, tam_bt, tempo_bt, tam_gul, tempo_gul);

        free(sol_bt);
        free(sol_gul);
        liberar_grafo(g);
    }

    return 0;
}