#include <stdio.h>
#include "conectividade.h"
#include "planaridade.h"

int main() {
    GrafoLista *g = criar_grafo(5);
    add_aresta(g, 0, 1);
    add_aresta(g, 1, 2);
    add_aresta(g, 2, 0);
    add_aresta(g, 1, 3);
    add_aresta(g, 3, 4);

    printf("--- Testes de Conectividade ---\n");
    dfs_articulacoes(g);
    detectar_pontes(g);

    printf("\n--- Testes de Planaridade ---\n");
    printf("Euler aprova? %d\n", eh_planar_euler(g));
    printf("Kuratowski aprova? %d\n", heuristica_kuratowski(g));

    GrafoLista *k5 = criar_grafo(5);
    for(int i=0; i<5; i++) {
        for(int j=i+1; j<5; j++) {
            add_aresta(k5, i, j);
        }
    }
    
    printf("\nGrafo K5 Completo:\n");
    printf("Euler aprova? %d\n", eh_planar_euler(k5));
    printf("Kuratowski aprova? %d\n", heuristica_kuratowski(k5));

    liberar_grafo(g);
    liberar_grafo(k5);
    return 0;
}