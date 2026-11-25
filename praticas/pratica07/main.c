#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bellman_ford.h"

int main() {
    int n1 = 5;
    GrafoListaPonderado *g1 = criar_grafo(n1);
    add_aresta(g1, 0, 1, 6);
    add_aresta(g1, 0, 3, 7);
    add_aresta(g1, 1, 2, 5);
    add_aresta(g1, 1, 3, 8);
    add_aresta(g1, 1, 4, -4);
    add_aresta(g1, 2, 1, -2);
    add_aresta(g1, 3, 2, -3);
    add_aresta(g1, 3, 4, 9);
    add_aresta(g1, 4, 0, 2);
    add_aresta(g1, 4, 2, 7);

    int *dist1 = (int*) malloc(n1 * sizeof(int));
    int *pred1 = (int*) malloc(n1 * sizeof(int));

    printf("--- Teste 1: Grafo sem ciclo negativo ---\n");
    if (bellman_ford(g1, 0, dist1, pred1)) {
        for (int i = 0; i < n1; i++) {
            printf("Vertice %d | Distancia: %d | Pred: %d\n", i, dist1[i], pred1[i]);
        }
    } else {
        reconstruir_ciclo_negativo(g1, dist1, pred1);
    }

    int n2 = 4;
    GrafoListaPonderado *g2 = criar_grafo(n2);
    add_aresta(g2, 0, 1, 4);
    add_aresta(g2, 1, 2, 3);
    add_aresta(g2, 2, 3, -6);
    add_aresta(g2, 3, 1, 1);

    int *dist2 = (int*) malloc(n2 * sizeof(int));
    int *pred2 = (int*) malloc(n2 * sizeof(int));

    printf("\n--- Teste 2: Grafo com ciclo negativo ---\n");
    if (bellman_ford(g2, 0, dist2, pred2)) {
        printf("Menores caminhos calculados com sucesso.\n");
    } else {
        reconstruir_ciclo_negativo(g2, dist2, pred2);
    }

    free(dist1); free(pred1); liberar_grafo(g1);
    free(dist2); free(pred2); liberar_grafo(g2);
    return 0;
}