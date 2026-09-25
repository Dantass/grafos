#ifndef CONECTIVIDADE_H
#define CONECTIVIDADE_H

typedef struct No {
    int destino;
    struct No *prox;
} No;

typedef struct {
    int n;
    int m; 
    No **adj;
} GrafoLista;

void dfs_articulacoes(GrafoLista *g);
void detectar_pontes(GrafoLista *g);

GrafoLista* criar_grafo(int n);
void add_aresta(GrafoLista *g, int u, int v);
void liberar_grafo(GrafoLista *g);

#endif