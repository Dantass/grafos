#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "dijkstra.h"

GrafoListaPonderado* criar_grafo(int n) {
    GrafoListaPonderado *g = (GrafoListaPonderado*) malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = (No**) calloc(n, sizeof(No*));
    return g;
}

void add_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v;
    nu->peso = peso;
    nu->prox = g->adj[u];
    g->adj[u] = nu;
}

void liberar_grafo(GrafoListaPonderado *g) {
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g);
}


MinHeap* criar_min_heap(int capacidade) {
    MinHeap *h = (MinHeap*) malloc(sizeof(MinHeap));
    h->dados = (NoHeap*) malloc(capacidade * sizeof(NoHeap));
    h->posicao = (int*) malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;
    for (int i = 0; i < capacidade; i++) {
        h->posicao[i] = -1;
    }
    return h;
}

static void trocar_nos(MinHeap *h, int i, int j) {
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;

    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void min_heapify(MinHeap *h, int idx) {
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia) {
        menor = esq;
    }
    if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia) {
        menor = dir;
    }

    if (menor != idx) {
        trocar_nos(h, idx, menor);
        min_heapify(h, menor);
    }
}

void inserir_min_heap(MinHeap *h, int vertice, int distancia) {
    if (h->tamanho == h->capacidade) return;

    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;

    while (i > 0 && h->dados[(i - 1) / 2].distancia > h->dados[i].distancia) {
        trocar_nos(h, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    NoHeap ultimo = h->dados[h->tamanho - 1];

    h->dados[0] = ultimo;
    h->posicao[raiz.vertice] = -1;
    h->posicao[ultimo.vertice] = 0;

    h->tamanho--;
    if (h->tamanho > 0) {
        min_heapify(h, 0);
    }

    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_dist) {
    int i = h->posicao[vertice];
    if (i == -1) return;

    h->dados[i].distancia = nova_dist;

    while (i > 0 && h->dados[(i - 1) / 2].distancia > h->dados[i].distancia) {
        trocar_nos(h, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void liberar_min_heap(MinHeap *h) {
    free(h->dados);
    free(h->posicao);
    free(h);
}


void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    MinHeap *h = criar_min_heap(g->n);

    for (int v = 0; v < g->n; v++) {
        dist[v] = INT_MAX;
        pred[v] = -1;
        inserir_min_heap(h, v, INT_MAX);
    }

    dist[origem] = 0;
    diminuir_chave(h, origem, 0);

    while (h->tamanho > 0) {
        NoHeap min_no = extrair_minimo(h);
        int u = min_no.vertice;

        if (dist[u] == INT_MAX) break;

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (h->posicao[v] != -1 && dist[u] + atual->peso < dist[v]) {
                dist[v] = dist[u] + atual->peso;
                pred[v] = u;
                diminuir_chave(h, v, dist[v]);
            }
            atual = atual->prox;
        }
    }

    liberar_min_heap(h);
}

void reconstruir_caminho(int origem, int destino, int *pred) {
    if (origem == destino) {
        printf("%d", origem);
        return;
    }
    if (pred[destino] == -1) {
        printf("Sem caminho de %d para %d", origem, destino);
        return;
    }
    reconstruir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}