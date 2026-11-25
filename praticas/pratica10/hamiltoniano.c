#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

GrafoHamilton* criar_grafo_hamilton(int n) {
    GrafoHamilton *g = (GrafoHamilton*) malloc(sizeof(GrafoHamilton));
    g->n = n;
    g->adj = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = (int*) calloc(n, sizeof(int));
    }
    return g;
}

void add_aresta_hamilton(GrafoHamilton *g, int u, int v) {
    if (u != v && u >= 0 && u < g->n && v >= 0 && v < g->n) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
    }
}

void liberar_grafo_hamilton(GrafoHamilton *g) {
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

static int bt_caminho(GrafoHamilton *g, int pos, int *caminho, int *visitado, int exigir_ciclo) {
    if (pos == g->n) {
        if (!exigir_ciclo) return 1;
        return g->adj[caminho[pos - 1]][caminho[0]];
    }

    int u = caminho[pos - 1];
    for (int v = 0; v < g->n; v++) {
        if (!visitado[v] && g->adj[u][v]) {
            visitado[v] = 1;
            caminho[pos] = v;

            if (bt_caminho(g, pos + 1, caminho, visitado, exigir_ciclo)) {
                return 1;
            }

            visitado[v] = 0;
        }
    }
    return 0;
}

int encontrar_caminho_hamiltoniano_bt(GrafoHamilton *g, int *caminho) {
    int *visitado = (int*) calloc(g->n, sizeof(int));

    for (int inicio = 0; inicio < g->n; inicio++) {
        visitado[inicio] = 1;
        caminho[0] = inicio;

        if (bt_caminho(g, 1, caminho, visitado, 0)) {
            free(visitado);
            return 1;
        }

        visitado[inicio] = 0;
    }

    free(visitado);
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(GrafoHamilton *g, int *ciclo) {
    if (g->n < 3) return 0;

    int *visitado = (int*) calloc(g->n, sizeof(int));
    visitado[0] = 1;
    ciclo[0] = 0;

    int encontrou = bt_caminho(g, 1, ciclo, visitado, 1);
    free(visitado);
    return encontrou;
}