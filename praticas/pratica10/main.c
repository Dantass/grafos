#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

static GrafoTSP* gerar_tsp_aleatorio(int n) {
    GrafoTSP *g = criar_grafo_tsp(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int peso = 1 + (rand() % 50);
            g->dist[i][j] = peso;
            g->dist[j][i] = peso;
        }
    }
    return g;
}

int main(void) {
    srand(42);

    printf("=== Parte 1: Reducao Hamiltoniano -> TSP ===\n");
    int n_ham = 5;
    GrafoHamilton *gh = criar_grafo_hamilton(n_ham);
    add_aresta_hamilton(gh, 0, 1);
    add_aresta_hamilton(gh, 1, 2);
    add_aresta_hamilton(gh, 2, 3);
    add_aresta_hamilton(gh, 3, 4);
    add_aresta_hamilton(gh, 4, 0);
    add_aresta_hamilton(gh, 1, 3);

    int *caminho_ham = (int*) malloc(n_ham * sizeof(int));
    int tem_caminho = encontrar_caminho_hamiltoniano_bt(gh, caminho_ham);
    int tem_ciclo = encontrar_ciclo_hamiltoniano_bt(gh, caminho_ham);

    GrafoTSP *tsp_reduzido = grafo_para_tsp(gh);
    int *rota_red = (int*) malloc(n_ham * sizeof(int));
    int custo_red = tsp_forca_bruta(tsp_reduzido, rota_red);

    printf("Caminho Hamiltoniano encontrado? %s\n", tem_caminho ? "Sim" : "Nao");
    printf("Ciclo Hamiltoniano encontrado?   %s (Custo TSP reduzido = %d, esperado = %d)\n\n",
           tem_ciclo ? "Sim" : "Nao", custo_red, n_ham);

    free(caminho_ham);
    free(rota_red);
    liberar_grafo_tsp(tsp_reduzido);
    liberar_grafo_hamilton(gh);

    printf("=== Parte 2: Experimento TSP (n = 5, 8, 10, 12) ===\n\n");
    int tamanhos[] = {5, 8, 10, 12};
    int num_testes = 4;

    printf("%-4s | %-9s | %-12s | %-9s | %-12s | %-9s | %-12s\n",
           "n", "Custo FB", "Tempo FB (s)", "Custo VMP", "Tempo VMP", "Custo Ins", "Tempo Ins");
    printf("-----+-----------+--------------+-----------+--------------+-----------+--------------\n");

    for (int t = 0; t < num_testes; t++) {
        int n = tamanhos[t];
        GrafoTSP *g = gerar_tsp_aleatorio(n);

        int *rota_fb = (int*) malloc(n * sizeof(int));
        int *rota_vmp = (int*) malloc(n * sizeof(int));
        int *rota_ins = (int*) malloc(n * sizeof(int));

        clock_t ini = clock();
        int custo_fb = tsp_forca_bruta(g, rota_fb);
        double tempo_fb = (double)(clock() - ini) / CLOCKS_PER_SEC;

        ini = clock();
        int custo_vmp = tsp_vizinho_mais_proximo(g, 0, rota_vmp);
        double tempo_vmp = (double)(clock() - ini) / CLOCKS_PER_SEC;

        ini = clock();
        int custo_ins = tsp_insercao(g, 0, rota_ins);
        double tempo_ins = (double)(clock() - ini) / CLOCKS_PER_SEC;

        printf("%-4d | %-9d | %-12.6f | %-9d | %-12.6f | %-9d | %-12.6f\n",
               n, custo_fb, tempo_fb, custo_vmp, tempo_vmp, custo_ins, tempo_ins);

        free(rota_fb);
        free(rota_vmp);
        free(rota_ins);
        liberar_grafo_tsp(g);
    }

    return 0;
}