#ifndef DAG_H
#define DAG_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

int* ordenacao_topologica_kahn(GrafoLista *g, int *tamanho);
int* ordenacao_topologica_dfs(GrafoLista *g, int *tamanho);
int eh_dag(GrafoLista *g);

#endif