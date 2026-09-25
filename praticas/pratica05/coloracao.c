#include <stdlib.h>
#include <stdio.h>
#include "coloracao.h"

GrafoLista* criar_grafo(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

void add_aresta(GrafoLista *g, int u, int v) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v; nu->prox = g->adj[u]; g->adj[u] = nu;
    No *nv = (No*) malloc(sizeof(No));
    nv->destino = u; nv->prox = g->adj[v]; g->adj[v] = nv;
}

void liberar_grafo(GrafoLista *g) {
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual) { No *temp = atual; atual = atual->prox; free(temp); }
    }
    free(g->adj); free(g);
}

int* coloracao_gulosa(GrafoLista *g, int *num_cores) {
    int *cor = (int*) malloc(g->n * sizeof(int));
    int *disponivel = (int*) malloc(g->n * sizeof(int));
    
    for (int i = 0; i < g->n; i++) cor[i] = -1;
    cor[0] = 0;
    *num_cores = 1;

    for (int u = 1; u < g->n; u++) {
        for (int i = 0; i < g->n; i++) disponivel[i] = 1;

        No *atual = g->adj[u];
        while (atual != NULL) {
            if (cor[atual->destino] != -1) {
                disponivel[cor[atual->destino]] = 0;
            }
            atual = atual->prox;
        }

        int c;
        for (c = 0; c < g->n; c++) {
            if (disponivel[c]) break;
        }
        cor[u] = c;
        if (c + 1 > *num_cores) *num_cores = c + 1;
    }
    
    free(disponivel);
    return cor;
}

int* coloracao_welsh_powell(GrafoLista *g, int *num_cores) {
    int *grau = (int*) calloc(g->n, sizeof(int));
    int *vertice = (int*) malloc(g->n * sizeof(int));
    int *cor = (int*) malloc(g->n * sizeof(int));
    
    for (int i = 0; i < g->n; i++) {
        cor[i] = -1;
        vertice[i] = i;
        No *atual = g->adj[i];
        while (atual) { grau[i]++; atual = atual->prox; }
    }

    for (int i = 0; i < g->n - 1; i++) {
        for (int j = 0; j < g->n - i - 1; j++) {
            if (grau[vertice[j]] < grau[vertice[j+1]]) {
                int temp = vertice[j];
                vertice[j] = vertice[j+1];
                vertice[j+1] = temp;
            }
        }
    }

    int cor_atual = 0;
    int coloridos = 0;
    
    while (coloridos < g->n) {
        for (int i = 0; i < g->n; i++) {
            int u = vertice[i];
            if (cor[u] == -1) {
                int pode_colorir = 1;
                No *atual = g->adj[u];
                while (atual) {
                    if (cor[atual->destino] == cor_atual) {
                        pode_colorir = 0;
                        break;
                    }
                    atual = atual->prox;
                }
                if (pode_colorir) {
                    cor[u] = cor_atual;
                    coloridos++;
                }
            }
        }
        cor_atual++;
    }
    
    *num_cores = cor_atual;
    free(grau); free(vertice);
    return cor;
}

int eh_bipartido(GrafoLista *g) {
    int *cor = (int*) malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) cor[i] = -1;
    
    int *fila = (int*) malloc(g->n * sizeof(int));
    int inicio = 0, fim = 0;

    for (int i = 0; i < g->n; i++) {
        if (cor[i] == -1) {
            cor[i] = 0;
            fila[fim++] = i;
            
            while (inicio < fim) {
                int u = fila[inicio++];
                No *atual = g->adj[u];
                
                while (atual) {
                    int v = atual->destino;
                    if (cor[v] == -1) {
                        cor[v] = 1 - cor[u];
                        fila[fim++] = v;
                    } else if (cor[v] == cor[u]) {
                        free(cor); free(fila);
                        return 0; 
                    }
                    atual = atual->prox;
                }
            }
        }
    }
    
    free(cor); free(fila);
    return 1;
}