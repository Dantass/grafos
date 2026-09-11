#include <stdio.h>
#include <stdlib.h>
#include "dag.h"

GrafoLista* criar_grafo(int n) {
    GrafoLista *g = (GrafoLista*) malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

void add_aresta_direcionada(GrafoLista *g, int u, int v) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v;
    nu->prox = g->adj[u];
    g->adj[u] = nu;
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

int main() {
    int n = 6;
    GrafoLista *g = criar_grafo(n);
    
    add_aresta_direcionada(g, 5, 2);
    add_aresta_direcionada(g, 5, 0);
    add_aresta_direcionada(g, 4, 0);
    add_aresta_direcionada(g, 4, 1);
    add_aresta_direcionada(g, 2, 3);
    add_aresta_direcionada(g, 3, 1);

    printf("--- Validacao do Grafo ---\n");
    printf("Eh DAG? %d\n\n", eh_dag(g));

    int tamanho;
    
    int *kahn = ordenacao_topologica_kahn(g, &tamanho);
    if (kahn != NULL) {
        printf("Ordem Kahn: ");
        for (int i = 0; i < tamanho; i++) printf("%d ", kahn[i]);
        printf("\n");
        free(kahn);
    }

    int *dfs_ord = ordenacao_topologica_dfs(g, &tamanho);
    if (dfs_ord != NULL) {
        printf("Ordem DFS:  ");
        for (int i = 0; i < tamanho; i++) printf("%d ", dfs_ord[i]);
        printf("\n");
        free(dfs_ord);
    }

    
    printf("\nAdicionando aresta 1 -> 5 (Criando ciclo)...\n");
    add_aresta_direcionada(g, 1, 5);
    
    printf("Eh DAG agora? %d\n", eh_dag(g));

    liberar_grafo(g);
    return 0;
}