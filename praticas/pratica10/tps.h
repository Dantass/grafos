#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

typedef struct {
    int n;
    int **dist;
} GrafoTSP;

GrafoTSP* criar_grafo_tsp(int n);
void liberar_grafo_tsp(GrafoTSP *g);

GrafoTSP* grafo_para_tsp(GrafoHamilton *g);

int tsp_forca_bruta(GrafoTSP *g, int *melhor_rota);
int tsp_vizinho_mais_proximo(GrafoTSP *g, int origem, int *rota);
int tsp_insercao(GrafoTSP *g, int origem, int *rota);

#endif