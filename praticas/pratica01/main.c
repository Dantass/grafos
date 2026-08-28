#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    int n = 5;

    // Testes Matriz de Adjacência
    GrafoMatriz *gm = criar_grafo_matriz(n);
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    
    printf("--- Matriz de Adjacencia ---\n");
    printf("Grau do vertice 0: %d\n", grau_matriz(gm, 0));
    printf("0 e 1 sao adjacentes? %d\n", sao_adjacentes_matriz(gm, 0, 1));
    
    remover_aresta_matriz(gm, 0, 1);
    printf("Apos remocao, 0 e 1 sao adjacentes? %d\n\n", sao_adjacentes_matriz(gm, 0, 1));
    liberar_grafo_matriz(gm);

    // Testes Lista de Adjacência
    GrafoLista *gl = criar_grafo_lista(n);
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    
    printf("--- Lista de Adjacencia ---\n");
    printf("Grau do vertice 0: %d\n", grau_lista(gl, 0));
    printf("0 e 1 sao adjacentes? %d\n", sao_adjacentes_lista(gl, 0, 1));
    
    remover_aresta_lista(gl, 0, 1);
    printf("Apos remocao, 0 e 1 sao adjacentes? %d\n", sao_adjacentes_lista(gl, 0, 1));
    liberar_grafo_lista(gl);

    return 0;
}