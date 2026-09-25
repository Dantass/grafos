#include <stdio.h>
#include <stdlib.h>
#include "conectividade.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

GrafoLista* criar_grafo(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->m = 0;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

void add_aresta(GrafoLista *g, int u, int v) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v; nu->prox = g->adj[u]; g->adj[u] = nu;
    
    No *nv = (No*) malloc(sizeof(No));
    nv->destino = u; nv->prox = g->adj[v]; g->adj[v] = nv;
    
    g->m++; 
}

void liberar_grafo(GrafoLista *g) {
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

static void dfs_tarjan_articulacoes(GrafoLista *g, int u, int *tempo, int *descoberta, int *low, int *pai, int *art) {
    int filhos = 0;
    descoberta[u] = low[u] = ++(*tempo);

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!descoberta[v]) {
            filhos++;
            pai[v] = u;
            dfs_tarjan_articulacoes(g, v, tempo, descoberta, low, pai, art);
            
            low[u] = MIN(low[u], low[v]);

            if (pai[u] == -1 && filhos > 1) art[u] = 1;
            if (pai[u] != -1 && low[v] >= descoberta[u]) art[u] = 1;
        } else if (v != pai[u]) {
            low[u] = MIN(low[u], descoberta[v]);
        }
        atual = atual->prox;
    }
}

void dfs_articulacoes(GrafoLista *g) {
    int *descoberta = (int*) calloc(g->n, sizeof(int));
    int *low = (int*) calloc(g->n, sizeof(int));
    int *pai = (int*) malloc(g->n * sizeof(int));
    int *art = (int*) calloc(g->n, sizeof(int));
    int tempo = 0;

    for (int i = 0; i < g->n; i++) pai[i] = -1;

    for (int i = 0; i < g->n; i++) {
        if (!descoberta[i]) dfs_tarjan_articulacoes(g, i, &tempo, descoberta, low, pai, art);
    }

    printf("Articulacoes: ");
    int achou = 0;
    for (int i = 0; i < g->n; i++) {
        if (art[i]) { printf("%d ", i); achou = 1; }
    }
    if (!achou) printf("Nenhuma");
    printf("\n");

    free(descoberta); free(low); free(pai); free(art);
}

static void dfs_tarjan_pontes(GrafoLista *g, int u, int *tempo, int *descoberta, int *low, int *pai) {
    descoberta[u] = low[u] = ++(*tempo);

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (!descoberta[v]) {
            pai[v] = u;
            dfs_tarjan_pontes(g, v, tempo, descoberta, low, pai);
            
            low[u] = MIN(low[u], low[v]);

            if (low[v] > descoberta[u]) {
                printf("Ponte encontrada: %d - %d\n", u, v);
            }
        } else if (v != pai[u]) {
            low[u] = MIN(low[u], descoberta[v]);
        }
        atual = atual->prox;
    }
}

void detectar_pontes(GrafoLista *g) {
    int *descoberta = (int*) calloc(g->n, sizeof(int));
    int *low = (int*) calloc(g->n, sizeof(int));
    int *pai = (int*) malloc(g->n * sizeof(int));
    int tempo = 0;

    for (int i = 0; i < g->n; i++) pai[i] = -1;

    printf("Verificando Pontes...\n");
    for (int i = 0; i < g->n; i++) {
        if (!descoberta[i]) dfs_tarjan_pontes(g, i, &tempo, descoberta, low, pai);
    }

    free(descoberta); free(low); free(pai);
}