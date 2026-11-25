#include <stdio.h>
#include <stdlib.h>
#include "cobertura.h"

GrafoMatriz* criar_grafo(int n) {
    GrafoMatriz *g = (GrafoMatriz*) malloc(sizeof(GrafoMatriz));
    g->n = n;
    g->adj = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->adj[i] = (int*) calloc(n, sizeof(int));
    }
    return g;
}

void add_aresta(GrafoMatriz *g, int u, int v) {
    if (u != v && u >= 0 && u < g->n && v >= 0 && v < g->n) {
        g->adj[u][v] = 1;
        g->adj[v][u] = 1;
    }
}

void liberar_grafo(GrafoMatriz *g) {
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

int eh_cobertura(GrafoMatriz *g, const int *selecionados) {
    for (int u = 0; u < g->n; u++) {
        for (int v = u + 1; v < g->n; v++) {
            if (g->adj[u][v] && !selecionados[u] && !selecionados[v]) {
                return 0;
            }
        }
    }
    return 1;
}

static void bt_recursivo(GrafoMatriz *g, int idx, int atual_tam, int *atual,
                         int *melhor_tam, int *melhor) {
    if (atual_tam >= *melhor_tam) {
        return;
    }

    if (idx == g->n) {
        *melhor_tam = atual_tam;
        for (int i = 0; i < g->n; i++) {
            melhor[i] = atual[i];
        }
        return;
    }

    atual[idx] = 1;
    bt_recursivo(g, idx + 1, atual_tam + 1, atual, melhor_tam, melhor);

    int pode_excluir = 1;
    for (int v = 0; v < idx; v++) {
        if (g->adj[idx][v] && !atual[v]) {
            pode_excluir = 0;
            break;
        }
    }

    if (pode_excluir) {
        atual[idx] = 0;
        bt_recursivo(g, idx + 1, atual_tam, atual, melhor_tam, melhor);
    }
}

int cobertura_minima_bt(GrafoMatriz *g, int *melhor_cobertura) {
    int *atual = (int*) calloc(g->n, sizeof(int));
    int melhor_tam = g->n + 1;

    for (int i = 0; i < g->n; i++) {
        melhor_cobertura[i] = 1;
    }

    bt_recursivo(g, 0, 0, atual, &melhor_tam, melhor_cobertura);

    free(atual);
    return melhor_tam;
}

int cobertura_gulosa(GrafoMatriz *g, int *cobertura) {
    int **adj_rest = (int**) malloc(g->n * sizeof(int*));
    int *grau = (int*) calloc(g->n, sizeof(int));
    int arestas_restantes = 0;

    for (int i = 0; i < g->n; i++) {
        cobertura[i] = 0;
        adj_rest[i] = (int*) malloc(g->n * sizeof(int));
        for (int j = 0; j < g->n; j++) {
            adj_rest[i][j] = g->adj[i][j];
            if (g->adj[i][j]) {
                grau[i]++;
                if (i < j) arestas_restantes++;
            }
        }
    }

    int tam = 0;
    while (arestas_restantes > 0) {
        int melhor_v = -1;
        int max_grau = 0;

        for (int i = 0; i < g->n; i++) {
            if (!cobertura[i] && grau[i] > max_grau) {
                max_grau = grau[i];
                melhor_v = i;
            }
        }

        if (melhor_v == -1) break;

        cobertura[melhor_v] = 1;
        tam++;

        for (int v = 0; v < g->n; v++) {
            if (adj_rest[melhor_v][v]) {
                adj_rest[melhor_v][v] = 0;
                adj_rest[v][melhor_v] = 0;
                grau[melhor_v]--;
                grau[v]--;
                arestas_restantes--;
            }
        }
    }

    for (int i = 0; i < g->n; i++) {
        free(adj_rest[i]);
    }
    free(adj_rest);
    free(grau);

    return tam;
}