#include <stdlib.h>
#include "dag.h"

typedef struct {
    int *dados;
    int inicio, fim, tamanho;
} Fila;

static Fila* criar_fila(int capacidade) {
    Fila *f = (Fila*) malloc(sizeof(Fila));
    f->dados = (int*) malloc(capacidade * sizeof(int));
    f->inicio = f->fim = f->tamanho = 0;
    return f;
}

static void enfileirar(Fila *f, int v) {
    f->dados[f->fim++] = v;
    f->tamanho++;
}

static int desenfileirar(Fila *f) {
    f->tamanho--;
    return f->dados[f->inicio++];
}

static void liberar_fila(Fila *f) {
    free(f->dados);
    free(f);
}

int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho) {
    int *grau_entrada = (int*) calloc(g->n, sizeof(int));
    
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            grau_entrada[atual->destino]++;
            atual = atual->prox;
        }
    }

    Fila *f = criar_fila(g->n);
    for (int i = 0; i < g->n; i++) {
        if (grau_entrada[i] == 0) enfileirar(f, i);
    }

    int *ordem = (int*) malloc(g->n * sizeof(int));
    int count = 0;

    while (f->tamanho > 0) {
        int u = desenfileirar(f);
        ordem[count++] = u;

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            grau_entrada[v]--;
            if (grau_entrada[v] == 0) enfileirar(f, v);
            atual = atual->prox;
        }
    }

    liberar_fila(f);
    free(grau_entrada);

    
    if (count != g->n) {
        free(ordem);
        *tamanho = 0;
        return NULL; 
    }

    *tamanho = count;
    return ordem;
}

static int dfs_visit(GrafoLista *g, int u, int *cor, int *pilha_saida, int *idx) {
    cor[u] = 1; 

    No *atual = g->adj[u];
    while (atual != NULL) {
        int v = atual->destino;
        if (cor[v] == 1) return 0; 
        if (cor[v] == 0) {
            if (!dfs_visit(g, v, cor, pilha_saida, idx)) return 0;
        }
        atual = atual->prox;
    }

    cor[u] = 2; 
    pilha_saida[(*idx)++] = u; 
    return 1;
}

int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho) {
    int *cor = (int*) calloc(g->n, sizeof(int));
    int *pilha_saida = (int*) malloc(g->n * sizeof(int));
    int idx = 0;

    for (int i = 0; i < g->n; i++) {
        if (cor[i] == 0) {
            if (!dfs_visit(g, i, cor, pilha_saida, &idx)) {
                free(cor);
                free(pilha_saida);
                *tamanho = 0;
                return NULL; 
            }
        }
    }
    
    free(cor);
    *tamanho = idx;

    int *resultado = (int*) malloc(g->n * sizeof(int));
    for(int i = 0; i < g->n; i++){
        resultado[i] = pilha_saida[g->n - 1 - i];
    }
    
    free(pilha_saida);
    return resultado;
}

int eh_dag(GrafoLista *g) {
    int tam;
    int *ordem = ordenacao_topologica_kahn(g, &tam);
    if (ordem == NULL) {
        return 0; 
    }
    free(ordem);
    return 1;
}