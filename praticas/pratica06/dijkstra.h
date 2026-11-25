#ifndef DIJKSTRA_H
#define DIJKSTRA_H

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int n;
    No **adj;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; 
} MinHeap;

GrafoListaPonderado* criar_grafo(int n);
void add_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);

MinHeap* criar_min_heap(int capacidade);
void inserir_min_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_dist);
void liberar_min_heap(MinHeap *h);

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif