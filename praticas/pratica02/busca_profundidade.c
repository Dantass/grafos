#include <stdlib.h>
#include "busca_profundidade.h"

Pilha* criar_pilha(int capacidade) {
    Pilha *p = (Pilha*) malloc(sizeof(Pilha));
    p->capacidade = capacidade;
    p->dados = (int*) malloc(capacidade * sizeof(int));
    p->topo = -1;
    return p;
}

void empilhar(Pilha *p, int valor) {
    if (p->topo < p->capacidade - 1) p->dados[++(p->topo)] = valor;
}

int desempilhar(Pilha *p) {
    return (p->topo >= 0) ? p->dados[(p->topo)--] : -1;
}

void liberar_pilha(Pilha *p) {
    free(p->dados);
    free(p);
}

void dfs_recursiva(GrafoLista *g, int u, int *visitado, int pai, int *ciclo) {
    visitado[u] = 1;
    No *atual = g->adj[u];
    
    while (atual != NULL) {
        int v = atual->destino;
        if (!visitado[v]) {
            dfs_recursiva(g, v, visitado, u, ciclo);
        } else if (v != pai) {
            *ciclo = 1; 
        }
        atual = atual->prox;
    }
}

int contar_componentes(GrafoLista *g) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    int componentes = 0, dummy_ciclo = 0;

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            componentes++;
            dfs_recursiva(g, i, visitado, -1, &dummy_ciclo);
        }
    }
    
    free(visitado);
    return componentes;
}

int tem_ciclo(GrafoLista *g) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    int ciclo = 0;

    for (int i = 0; i < g->n; i++) {
        if (!visitado[i]) {
            dfs_recursiva(g, i, visitado, -1, &ciclo);
            if (ciclo) break;
        }
    }
    
    free(visitado);
    return ciclo;
}