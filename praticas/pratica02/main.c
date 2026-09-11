#include <stdio.h>
#include <stdlib.h>
#include "busca_largura.h"
#include "busca_profundidade.h"

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

int main() {
    GrafoLista *g = criar_grafo(5);
    add_aresta(g, 0, 1);
    add_aresta(g, 1, 2);
    add_aresta(g, 3, 4);

    printf("--- Testes BFS ---\n");
    int dist[5], pred[5];
    bfs(g, 0, dist, pred);
    printf("Distancia 0 ate 2: %d\n", dist[2]);
    printf("Eh bipartido: %d\n", eh_bipartido(g));

    printf("\n--- Testes DFS ---\n");
    printf("Componentes conexos: %d\n", contar_componentes(g));
    printf("Tem ciclo: %d\n", tem_ciclo(g));

    printf("\nAdicionando aresta 0-2 (Forcando ciclo e imparidade)...\n");
    add_aresta(g, 0, 2);
    
    printf("Tem ciclo agora: %d\n", tem_ciclo(g));
    printf("Eh bipartido agora: %d\n", eh_bipartido(g));

    liberar_grafo(g);
    return 0;
}