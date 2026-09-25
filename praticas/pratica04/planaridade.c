#include <stdio.h>
#include "planaridade.h"

int eh_planar_euler(GrafoLista *g) {
    if (g->n < 3) return 1; 
    
    if (g->m > 3 * g->n - 6) {
        return 0; 
    }
    return 1;
}

int heuristica_kuratowski(GrafoLista *g) {
    if (g->n > 10) return eh_planar_euler(g);
    
    if (g->n >= 5 && g->m >= 10) {
        int v_grau4 = 0;
        for (int i = 0; i < g->n; i++) {
            int grau = 0;
            No *atual = g->adj[i];
            while(atual) { grau++; atual = atual->prox; }
            if (grau >= 4) v_grau4++;
        }
        if (v_grau4 >= 5) {
            printf("Heuristica: Possivel subdivisao de K5 detectada.\n");
            return 0;
        }
    }

    if (g->n >= 6 && g->m >= 9) {
        int v_grau3 = 0;
        for (int i = 0; i < g->n; i++) {
            int grau = 0;
            No *atual = g->adj[i];
            while(atual) { grau++; atual = atual->prox; }
            if (grau >= 3) v_grau3++;
        }
        if (v_grau3 >= 6) {
            printf("Heuristica: Possivel subdivisao de K3,3 detectada.\n");
            return 0;
        }
    }
    
    return eh_planar_euler(g);
}