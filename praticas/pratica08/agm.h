#ifndef AGM_H
#define AGM_H

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

typedef struct No {
    int destino;
    int peso;
    struct No *prox;
} No;

typedef struct {
    int n;
    int m;
    No **adj;
    Aresta *arestas;
    int cap_arestas;
} GrafoPonderado;

GrafoPonderado* criar_grafo(int n, int cap_arestas);
void add_aresta(GrafoPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoPonderado *g);

DSU* criar_dsu(int n);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);
void liberar_dsu(DSU *d);

int kruskal(GrafoPonderado *g, Aresta *agm_arestas, int *num_arestas);
int prim(GrafoPonderado *g, int inicio, int *pai_agm);

#endif