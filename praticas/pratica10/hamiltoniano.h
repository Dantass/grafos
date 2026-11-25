#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

typedef struct {
    int n;
    int **adj;
} GrafoHamilton;

GrafoHamilton* criar_grafo_hamilton(int n);
void add_aresta_hamilton(GrafoHamilton *g, int u, int v);
void liberar_grafo_hamilton(GrafoHamilton *g);

int encontrar_caminho_hamiltoniano_bt(GrafoHamilton *g, int *caminho);
int encontrar_ciclo_hamiltoniano_bt(GrafoHamilton *g, int *ciclo);

#endif