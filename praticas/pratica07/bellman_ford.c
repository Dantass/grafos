#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "bellman_ford.h"

GrafoListaPonderado* criar_grafo(int n) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

void add_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v;
    nu->peso = peso;
    nu->prox = g->adj[u];
    g->adj[u] = nu;
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    for (int i = 0; i < g->n; i++) {
        dist[i] = INT_MAX;
        pred[i] = -1;
    }
    dist[origem] = 0;

    for (int passo = 0; passo < g->n - 1; passo++) {
        int atualizou = 0;
        for (int u = 0; u < g->n; u++) {
            if (dist[u] == INT_MAX) continue;

            No *atual = g->adj[u];
            while (atual != NULL) {
                int v = atual->destino;
                if (dist[u] + atual->peso < dist[v]) {
                    dist[v] = dist[u] + atual->peso;
                    pred[v] = u;
                    atualizou = 1;
                }
                atual = atual->prox;
            }
        }
        if (!atualizou) break;
    }

    for (int u = 0; u < g->n; u++) {
        if (dist[u] == INT_MAX) continue;

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (dist[u] + atual->peso < dist[v]) {
                pred[v] = u;
                return 0;
            }
            atual = atual->prox;
        }
    }

    return 1; 
}

void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred) {
    int x = -1;

    for (int u = 0; u < g->n && x == -1; u++) {
        if (dist[u] == INT_MAX) continue;

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (dist[u] + atual->peso < dist[v]) {
                pred[v] = u;
                x = v;
                break;
            }
            atual = atual->prox;
        }
    }

    if (x == -1) {
        printf("Nenhum ciclo negativo encontrado.\n");
        return;
    }

    for (int i = 0; i < g->n; i++) {
        x = pred[x];
    }

    int *ciclo = (int*) malloc((g->n + 1) * sizeof(int));
    int tam = 0;
    int atual = x;

    do {
        ciclo[tam++] = atual;
        atual = pred[atual];
    } while (atual != x && tam < g->n);
    ciclo[tam++] = x;

    printf("Ciclo negativo detectado: ");
    for (int i = tam - 1; i >= 0; i--) {
        printf("%d", ciclo[i]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");

    free(ciclo);
}