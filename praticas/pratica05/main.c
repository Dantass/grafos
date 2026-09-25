#include <stdio.h>
#include <stdlib.h>
#include "coloracao.h"

int main() {
    GrafoLista *g = criar_grafo(5);
    add_aresta(g, 0, 1);
    add_aresta(g, 1, 2);
    add_aresta(g, 2, 3);
    add_aresta(g, 3, 0);
    add_aresta(g, 4, 0);
    add_aresta(g, 4, 1);

    printf("--- Testes de Coloração ---\n");
    
    int num_cores_gulosa;
    int *cores_gulosa = coloracao_gulosa(g, &num_cores_gulosa);
    printf("Coloracao Gulosa usou %d cores.\n", num_cores_gulosa);
    
    int num_cores_wp;
    int *cores_wp = coloracao_welsh_powell(g, &num_cores_wp);
    printf("Coloracao Welsh-Powell usou %d cores.\n", num_cores_wp);
    
    printf("O grafo eh bipartido (2-coloracao)? %d\n", eh_bipartido(g));

    free(cores_gulosa);
    free(cores_wp);
    liberar_grafo(g);
    
    return 0;
}