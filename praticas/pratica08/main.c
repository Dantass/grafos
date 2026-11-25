#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

int main() {
    int n = 5;
    GrafoPonderado *g = criar_grafo(n, 10);

    add_aresta(g, 0, 1, 2);
    add_aresta(g, 0, 3, 6);
    add_aresta(g, 1, 2, 3);
    add_aresta(g, 1, 3, 8);
    add_aresta(g, 1, 4, 5);
    add_aresta(g, 2, 4, 7);
    add_aresta(g, 3, 4, 9);

    Aresta *agm_arestas = (Aresta*) malloc((n - 1) * sizeof(Aresta));
    int num_arestas = 0;
    int custo_kruskal = kruskal(g, agm_arestas, &num_arestas);

    printf("--- Algoritmo de Kruskal ---\n");
    printf("Custo Total AGM: %d\n", custo_kruskal);
    for (int i = 0; i < num_arestas; i++) {
        printf("Aresta: %d - %d (peso %d)\n", agm_arestas[i].u, agm_arestas[i].v, agm_arestas[i].peso);
    }

    int *pai_agm = (int*) malloc(n * sizeof(int));
    int custo_prim = prim(g, 0, pai_agm);

    printf("\n--- Algoritmo de Prim ---\n");
    printf("Custo Total AGM: %d\n", custo_prim);
    for (int i = 1; i < n; i++) {
        printf("Aresta: %d - %d\n", pai_agm[i], i);
    }

    free(agm_arestas);
    free(pai_agm);
    liberar_grafo(g);
    return 0;
}