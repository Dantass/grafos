#ifndef COBERTURA_H
#define COBERTURA_H

typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

GrafoMatriz* criar_grafo(int n);
void add_aresta(GrafoMatriz *g, int u, int v);
void liberar_grafo(GrafoMatriz *g);

int eh_cobertura(GrafoMatriz *g, const int *selecionados);
int cobertura_minima_bt(GrafoMatriz *g, int *melhor_cobertura);
int cobertura_gulosa(GrafoMatriz *g, int *cobertura);

#endif