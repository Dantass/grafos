#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijkstra.h"

int main() {
    int n = 5;
    GrafoListaPonderado *g = criar_grafo(n);

    add_aresta(g, 0, 1, 10);
    add_aresta(g, 0, 3, 5);
    add_aresta(g, 1, 2, 1);
    add_aresta(g, 1, 3, 2);
    add_aresta(g, 2, 4, 4);
    add_aresta(g, 3, 1, 3);
    add_aresta(g, 3, 2, 9);
    add_aresta(g, 3, 4, 2);
    add_aresta(g, 4, 0, 7);
    add_aresta(g, 4, 2, 6);

    int *dist = (int*) malloc(n * sizeof(int));
    int *pred = (int*) malloc(n * sizeof(int));

    int origem = 0;
    dijkstra(g, origem, dist, pred);

    printf("--- Menores Caminhos a partir do vertice %d ---\n", origem);
    for (int i = 0; i < n; i++) {
        printf("Destino %d | Distancia: ", i);
        if (dist[i] == INT_MAX) {
            printf("INF | Caminho: Inacessivel\n");
        } else {
            printf("%2d  | Caminho: ", dist[i]);
            reconstruir_caminho(origem, i, pred);
            printf("\n");
        }
    }

    free(dist);
    free(pred);
    liberar_grafo(g);
    return 0;
}