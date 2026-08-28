#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

// Função auxiliar interna para inserir na lista
static void inserir_no(GrafoLista *g, int origem, int destino) {
    No *novo = (No*) malloc(sizeof(No));
    novo->destino = destino;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        inserir_no(g, u, v);
        if (u != v) {
            inserir_no(g, v, u); // Grafo simples
        }
    }
}

// Função auxiliar interna para remover da lista
static void remover_no(GrafoLista *g, int origem, int destino) {
    No *atual = g->adj[origem];
    No *ant = NULL;
    
    while (atual != NULL && atual->destino != destino) {
        ant = atual;
        atual = atual->prox;
    }
    
    if (atual != NULL) {
        if (ant == NULL) {
            g->adj[origem] = atual->prox;
        } else {
            ant->prox = atual->prox;
        }
        free(atual);
    }
}

void remover_aresta_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        remover_no(g, u, v);
        if (u != v) {
            remover_no(g, v, u);
        }
    }
}

int grau_lista(GrafoLista *g, int v) {
    int grau = 0;
    if (v >= 0 && v < g->n) {
        No *atual = g->adj[v];
        while (atual != NULL) {
            grau++;
            atual = atual->prox;
        }
    }
    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v) {
    if (u >= 0 && u < g->n && v >= 0 && v < g->n) {
        No *atual = g->adj[u];
        while (atual != NULL) {
            if (atual->destino == v) {
                return 1;
            }
            atual = atual->prox;
        }
    }
    return 0;
}

void liberar_grafo_lista(GrafoLista *g) {
    if (g != NULL) {
        for (int i = 0; i < g->n; i++) {
            No *atual = g->adj[i];
            while (atual != NULL) {
                No *prox = atual->prox;
                free(atual);
                atual = prox;
            }
        }
        free(g->adj);
        free(g);
    }
}