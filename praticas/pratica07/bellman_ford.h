#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoListaPonderado;

GrafoListaPonderado* criar_grafo(int n);
void add_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred);

#endif