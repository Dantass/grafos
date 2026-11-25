#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "agm.h"


GrafoPonderado* criar_grafo(int n, int cap_arestas) {
    GrafoPonderado *g = (GrafoPonderado*) malloc(sizeof(GrafoPonderado));
    g->n = n;
    g->m = 0;
    g->cap_arestas = cap_arestas;
    g->adj = (No**) calloc(n, sizeof(No*));
    g->arestas = (Aresta*) malloc(cap_arestas * sizeof(Aresta));
    return g;
}

void add_aresta(GrafoPonderado *g, int u, int v, int peso) {
    No *nu = (No*) malloc(sizeof(No));
    nu->destino = v; nu->peso = peso; nu->prox = g->adj[u];
    g->adj[u] = nu;

    No *nv = (No*) malloc(sizeof(No));
    nv->destino = u; nv->peso = peso; nv->prox = g->adj[v];
    g->adj[v] = nv;

    if (g->m < g->cap_arestas) {
        g->arestas[g->m].u = u;
        g->arestas[g->m].v = v;
        g->arestas[g->m].peso = peso;
        g->m++;
    }
}

void liberar_grafo(GrafoPonderado *g) {
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(g->adj);
    free(g->arestas);
    free(g);
}


DSU* criar_dsu(int n) {
    DSU *d = (DSU*) malloc(sizeof(DSU));
    d->n = n;
    d->pai = (int*) malloc(n * sizeof(int));
    d->rank = (int*) calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) {
        d->pai[i] = i;
    }
    return d;
}

int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x == raiz_y) return 0;

    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }
    return 1;
}

void liberar_dsu(DSU *d) {
    free(d->pai);
    free(d->rank);
    free(d);
}


static int comparar_arestas(const void *a, const void *b) {
    const Aresta *ea = (const Aresta*) a;
    const Aresta *eb = (const Aresta*) b;
    return (ea->peso > eb->peso) - (ea->peso < eb->peso);
}

int kruskal(GrafoPonderado *g, Aresta *agm_arestas, int *num_arestas) {
    Aresta *ordenadas = (Aresta*) malloc(g->m * sizeof(Aresta));
    for (int i = 0; i < g->m; i++) {
        ordenadas[i] = g->arestas[i];
    }

    qsort(ordenadas, g->m, sizeof(Aresta), comparar_arestas);

    DSU *dsu = criar_dsu(g->n);
    int custo_total = 0;
    *num_arestas = 0;

    for (int i = 0; i < g->m && *num_arestas < g->n - 1; i++) {
        if (unir(dsu, ordenadas[i].u, ordenadas[i].v)) {
            agm_arestas[(*num_arestas)++] = ordenadas[i];
            custo_total += ordenadas[i].peso;
        }
    }

    liberar_dsu(dsu);
    free(ordenadas);
    return custo_total;
}


typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int *posicao;
    int tamanho;
} MinHeap;

static MinHeap* criar_heap(int n) {
    MinHeap *h = (MinHeap*) malloc(sizeof(MinHeap));
    h->dados = (NoHeap*) malloc(n * sizeof(NoHeap));
    h->posicao = (int*) malloc(n * sizeof(int));
    h->tamanho = n;
    for (int i = 0; i < n; i++) {
        h->dados[i].vertice = i;
        h->dados[i].chave = INT_MAX;
        h->posicao[i] = i;
    }
    return h;
}

static void trocar_heap(MinHeap *h, int i, int j) {
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;
    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void heapify_down(MinHeap *h, int idx) {
    int menor = idx;
    int esq = 2 * idx + 1;
    int dir = 2 * idx + 2;

    if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave) menor = esq;
    if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave) menor = dir;

    if (menor != idx) {
        trocar_heap(h, idx, menor);
        heapify_down(h, menor);
    }
}

static void diminuir_chave_heap(MinHeap *h, int v, int nova_chave) {
    int i = h->posicao[v];
    h->dados[i].chave = nova_chave;
    while (i > 0 && h->dados[(i - 1) / 2].chave > h->dados[i].chave) {
        trocar_heap(h, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

static NoHeap extrair_min_heap(MinHeap *h) {
    NoHeap raiz = h->dados[0];
    NoHeap ultimo = h->dados[h->tamanho - 1];
    h->dados[0] = ultimo;
    h->posicao[raiz.vertice] = -1;
    h->posicao[ultimo.vertice] = 0;
    h->tamanho--;
    if (h->tamanho > 0) heapify_down(h, 0);
    return raiz;
}

int prim(GrafoPonderado *g, int inicio, int *pai_agm) {
    int *chave = (int*) malloc(g->n * sizeof(int));
    MinHeap *h = criar_heap(g->n);

    for (int i = 0; i < g->n; i++) {
        chave[i] = INT_MAX;
        pai_agm[i] = -1;
    }

    chave[inicio] = 0;
    diminuir_chave_heap(h, inicio, 0);

    int custo_total = 0;

    while (h->tamanho > 0) {
        NoHeap min_no = extrair_min_heap(h);
        int u = min_no.vertice;

        if (chave[u] == INT_MAX) break;
        custo_total += chave[u];

        No *atual = g->adj[u];
        while (atual != NULL) {
            int v = atual->destino;
            if (h->posicao[v] != -1 && atual->peso < chave[v]) {
                chave[v] = atual->peso;
                pai_agm[v] = u;
                diminuir_chave_heap(h, v, chave[v]);
            }
            atual = atual->prox;
        }
    }

    free(chave);
    free(h->dados);
    free(h->posicao);
    free(h);
    return custo_total;
}