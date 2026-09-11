#ifndef BUSCA_LARGURA_H
#define BUSCA_LARGURA_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoLista;

typedef struct {
    int *dados;
    int capacidade, inicio, fim, tamanho;
} Fila;

void bfs(GrafoLista *g, int origem, int *dist, int *pred);
int eh_bipartido(GrafoLista *g);

#endif