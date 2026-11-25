#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "tps.h"

GrafoTSP* criar_grafo_tsp(int n) {
    GrafoTSP *g = (GrafoTSP*) malloc(sizeof(GrafoTSP));
    g->n = n;
    g->dist = (int**) malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        g->dist[i] = (int*) calloc(n, sizeof(int));
    }
    return g;
}

void liberar_grafo_tsp(GrafoTSP *g) {
    for (int i = 0; i < g->n; i++) {
        free(g->dist[i]);
    }
    free(g->dist);
    free(g);
}

GrafoTSP* grafo_para_tsp(GrafoHamilton *g) {
    GrafoTSP *tsp = criar_grafo_tsp(g->n);
    for (int i = 0; i < g->n; i++) {
        for (int j = 0; j < g->n; j++) {
            if (i == j) {
                tsp->dist[i][j] = 0;
            } else {
                tsp->dist[i][j] = g->adj[i][j] ? 1 : 2;
            }
        }
    }
    return tsp;
}

static void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void permutar_tsp(GrafoTSP *g, int *rota, int idx, int custo_atual,
                         int *melhor_custo, int *melhor_rota) {
    if (custo_atual >= *melhor_custo) {
        return;
    }

    if (idx == g->n) {
        int custo_total = custo_atual + g->dist[rota[g->n - 1]][rota[0]];
        if (custo_total < *melhor_custo) {
            *melhor_custo = custo_total;
            for (int i = 0; i < g->n; i++) {
                melhor_rota[i] = rota[i];
            }
        }
        return;
    }

    for (int i = idx; i < g->n; i++) {
        trocar(&rota[idx], &rota[i]);
        int novo_custo = custo_atual + g->dist[rota[idx - 1]][rota[idx]];
        permutar_tsp(g, rota, idx + 1, novo_custo, melhor_custo, melhor_rota);
        trocar(&rota[idx], &rota[i]);
    }
}

int tsp_forca_bruta(GrafoTSP *g, int *melhor_rota) {
    int *rota = (int*) malloc(g->n * sizeof(int));
    for (int i = 0; i < g->n; i++) {
        rota[i] = i;
        melhor_rota[i] = i;
    }

    int melhor_custo = INT_MAX;
    permutar_tsp(g, rota, 1, 0, &melhor_custo, melhor_rota);

    free(rota);
    return melhor_custo;
}

int tsp_vizinho_mais_proximo(GrafoTSP *g, int origem, int *rota) {
    int *visitado = (int*) calloc(g->n, sizeof(int));
    int atual = origem;
    rota[0] = atual;
    visitado[atual] = 1;
    int custo_total = 0;

    for (int passo = 1; passo < g->n; passo++) {
        int prox = -1;
        int menor_dist = INT_MAX;

        for (int v = 0; v < g->n; v++) {
            if (!visitado[v] && g->dist[atual][v] < menor_dist) {
                menor_dist = g->dist[atual][v];
                prox = v;
            }
        }

        rota[passo] = prox;
        visitado[prox] = 1;
        custo_total += menor_dist;
        atual = prox;
    }

    custo_total += g->dist[atual][origem];
    free(visitado);
    return custo_total;
}

int tsp_insercao(GrafoTSP *g, int origem, int *rota) {
    int *na_rota = (int*) calloc(g->n, sizeof(int));
    int tam = 1;
    rota[0] = origem;
    na_rota[origem] = 1;

    int segundo = -1;
    int menor_d = INT_MAX;
    for (int v = 0; v < g->n; v++) {
        if (!na_rota[v] && g->dist[origem][v] < menor_d) {
            menor_d = g->dist[origem][v];
            segundo = v;
        }
    }
    if (segundo != -1) {
        rota[tam++] = segundo;
        na_rota[segundo] = 1;
    }

    while (tam < g->n) {
        int k_escolhido = -1;
        int dist_min_ciclo = INT_MAX;

        for (int k = 0; k < g->n; k++) {
            if (na_rota[k]) continue;
            for (int i = 0; i < tam; i++) {
                int u = rota[i];
                if (g->dist[u][k] < dist_min_ciclo) {
                    dist_min_ciclo = g->dist[u][k];
                    k_escolhido = k;
                }
            }
        }

        int melhor_pos = 0;
        int menor_aumento = INT_MAX;

        for (int i = 0; i < tam; i++) {
            int u = rota[i];
            int v = rota[(i + 1) % tam];
            int aumento = g->dist[u][k_escolhido] + g->dist[k_escolhido][v] - g->dist[u][v];
            if (aumento < menor_aumento) {
                menor_aumento = aumento;
                melhor_pos = i + 1;
            }
        }

        for (int i = tam; i > melhor_pos; i--) {
            rota[i] = rota[i - 1];
        }
        rota[melhor_pos] = k_escolhido;
        na_rota[k_escolhido] = 1;
        tam++;
    }

    int custo_total = 0;
    for (int i = 0; i < g->n; i++) {
        int u = rota[i];
        int v = rota[(i + 1) % g->n];
        custo_total += g->dist[u][v];
    }

    free(na_rota);
    return custo_total;
}