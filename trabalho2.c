#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAXN 1000

typedef struct node *link;
struct node { int w, custo; link next; };

typedef struct Graph *Graph;
struct Graph {
    int v, e;
    link *adj;
    bool *temPosto;
    bool *temRestaurante;
};

typedef struct {
    int local, numPedidos, *destinoPedido;
} Restaurante;

typedef struct {
    int v, w, custo;
} Edge;

Edge EDGE(int v, int w, int custo) {
    Edge e = {v, w, custo};
    return e;
}

link New(int w, int custo, link next) {
    link x = malloc(sizeof(*x));
    if (!x) return NULL;
    x->w = w; x->custo = custo; x->next = next;
    return x;
}

Graph GRAPHInit(int v) {
    Graph g = malloc(sizeof(*g));
    g->v = v; g->e = 0;
    g->adj = calloc(v, sizeof(link));
    g->temPosto = calloc(v, sizeof(bool));
    g->temRestaurante = calloc(v, sizeof(bool));
    return g;
}

void GRAPHInsert(Graph g, Edge e) {
    g->adj[e.v] = New(e.w, e.custo, g->adj[e.v]);
    g->e++;
}

void Dijkstra(Graph G, int s, int *pa, int *dist) {
    bool vis[MAXN] = {0};
    for (int i = 0; i < G->v; i++) {
        dist[i] = INT_MAX;
        pa[i] = -1;
    }
    dist[s] = 0;

    while (1) {
        int u = -1;
        for (int i = 0; i < G->v; i++) {
            if (!vis[i] && (u == -1 || dist[i] < dist[u]))
                u = i;
        }
        if (u == -1 || dist[u] == INT_MAX) break;
        vis[u] = true;
        for (link p = G->adj[u]; p; p = p->next) {
            if (dist[u] + p->custo < dist[p->w]) {
                dist[p->w] = dist[u] + p->custo;
                pa[p->w] = u;
            }
        }
    }
}

int N, M, H, T, I, C, P, Q;
Restaurante *restaurantes;
Graph G;

int mochila[MAXN], mochilaAtual = 0;
int posicaoAtual, combustivelAtual;

void emitir(const char *fmt, int x) {
    if (x >= 0) printf(fmt, x + 1);
    else printf("%s", fmt);
    printf("\n");
    fflush(stdout);
}

int ler() {
    int r; scanf("%d", &r); return r;
}

bool moverPara(int destino, int *pai, int *dist) {
    if (dist[destino] > combustivelAtual) {
        int melhorPosto = -1, menorCusto = INT_MAX;
        for (int i = 0; i < N; i++) {
            if (!G->temPosto[i]) continue;
            if (dist[i] <= combustivelAtual && dist[i] < menorCusto) {
                melhorPosto = i;
                menorCusto = dist[i];
            }
        }
        if (melhorPosto != -1) {
            moverPara(melhorPosto, pai, dist);
            emitir("a", -1);
            combustivelAtual = T;
            return moverPara(destino, pai, dist);
        }
        return false;
    }

    if (pai[destino] != -1) moverPara(pai[destino], pai, dist);
    emitir("m %d", destino);
    int r = ler();
    if (r == 1) {
        for (link a = G->adj[posicaoAtual]; a; a = a->next)
            if (a->w == destino) combustivelAtual -= a->custo;
        posicaoAtual = destino;

        if (G->temPosto[posicaoAtual]) {
            emitir("a", -1);
            combustivelAtual = T;
            ler();
        }

        return true;
    }
    return false;
}

bool pegarPedido(int destino) {
    emitir("p %d", destino);
    int r = ler();
    if (r == 1) mochila[mochilaAtual++] = destino;
    return r == 1;
}

bool entregarPedido(int destino) {
    emitir("e %d", destino);
    int r = ler();
    if (r == 1) {
        for (int i = 0; i < mochilaAtual; i++)
            if (mochila[i] == destino) {
                for (int j = i; j < mochilaAtual - 1; j++)
                    mochila[j] = mochila[j + 1];
                mochilaAtual--;
                break;
            }
    }
    return r == 1;
}

void encerrar() {
    emitir("x", -1);
    ler();
}

int destinoMaisProximo(int *dist) {
    int melhor = -1, melhorCusto = INT_MAX;
    for (int i = 0; i < Q; i++) {
        if (restaurantes[i].numPedidos > 0) {
            int local = restaurantes[i].local;
            if (dist[local] < melhorCusto) {
                melhorCusto = dist[local];
                melhor = i;
            }
        }
    }
    return melhor;
}

int main() {
    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    G = GRAPHInit(N);
    for (int i = 0; i < P; i++) {
        int p; scanf("%d", &p); G->temPosto[p - 1] = true;
    }
    for (int i = 0; i < M; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        GRAPHInsert(G, EDGE(u - 1, v - 1, w));
    }
    scanf("%d", &Q);
    restaurantes = malloc(Q * sizeof(Restaurante));
    for (int i = 0; i < Q; i++) {
        int r, k; scanf("%d %d", &r, &k);
        G->temRestaurante[r - 1] = true;
        restaurantes[i].local = r - 1;
        restaurantes[i].numPedidos = k;
        restaurantes[i].destinoPedido = malloc(k * sizeof(int));
        for (int j = 0; j < k; j++) {
            scanf("%d", &restaurantes[i].destinoPedido[j]);
            restaurantes[i].destinoPedido[j]--;
        }
    }
    posicaoAtual = H - 1;
    combustivelAtual = I;

    while (1) {
        int pai[MAXN], dist[MAXN];
        Dijkstra(G, posicaoAtual, pai, dist);

        int idx = destinoMaisProximo(dist);
        if (idx == -1) break;
        int rlocal = restaurantes[idx].local;
        moverPara(rlocal, pai, dist);

        while (restaurantes[idx].numPedidos > 0 && mochilaAtual < C) {
            int melhorD = -1, melhorDist = INT_MAX;
            for (int i = 0; i < restaurantes[idx].numPedidos; i++) {
                int d = restaurantes[idx].destinoPedido[i];
                Dijkstra(G, rlocal, pai, dist);
                if (dist[d] < melhorDist) {
                    melhorDist = dist[d];
                    melhorD = i;
                }
            }
            if (melhorD == -1) break;
            int destino = restaurantes[idx].destinoPedido[melhorD];
            if (!pegarPedido(destino)) break;
            for (int j = melhorD + 1; j < restaurantes[idx].numPedidos; j++)
                restaurantes[idx].destinoPedido[j - 1] = restaurantes[idx].destinoPedido[j];
            restaurantes[idx].numPedidos--;
        }

        for (int i = 0; i < mochilaAtual;) {
            int d = mochila[i];
            Dijkstra(G, posicaoAtual, pai, dist);
            moverPara(d, pai, dist);
            if (entregarPedido(d)) continue;
            i++;
        }
    }

    int pai[MAXN], dist[MAXN];
    Dijkstra(G, posicaoAtual, pai, dist);
    moverPara(H - 1, pai, dist);
    encerrar();
    return 0;
}