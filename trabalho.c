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

typedef struct {
    int *pedidos;       // Array de pedidos na mochila
    int num_pedidos;    // Quantidade atual de pedidos
    int capacidade;     // Capacidade máxima da mochila
} Mochila;

typedef struct {
    int posicao;
    int combustivel;
    Mochila mochila;
} Entregador;

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
    g->adj[e.w] = New(e.v, e.custo, g->adj[e.w]);
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
    int r;
    if (scanf("%d", &r) != 1) exit(1);
    return r;
}

bool moverPara(int destino, int *pai, int *dist) {
    if (destino < 0 || destino >= N) return false;

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
            combustivelAtual = ler();
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
            combustivelAtual = ler();
        }

        return true;
    }
    return false;
}

bool pegarPedido(int destino) {
    if (destino < 0 || destino >= N) return false;
    emitir("p %d", destino);
    int r = ler();
    if (r == 1) mochila[mochilaAtual++] = destino;
    return r == 1;
}

bool entregarPedido(int destino) {
    if (destino < 0 || destino >= N) return false;
    emitir("e %d", destino);
    int r = ler();
    if (r == 1) {
        for (int i = 0; i < mochilaAtual; i++) {
            if (mochila[i] == destino) {
                for (int j = i; j < mochilaAtual - 1; j++)
                    mochila[j] = mochila[j + 1];
                mochilaAtual--;
                break;
            }
        }
    }
    return r == 1;
}

int encontrarEntregaMaisProxima(int *pai, int *dist) {
    int distLocal[MAXN], paiLocal[MAXN];
    Dijkstra(G, posicaoAtual, paiLocal, distLocal);
    int melhor = -1, menorDist = INT_MAX;
    for (int i = 0; i < mochilaAtual; i++) {
        int destino = mochila[i];
        if (destino >= 0 && destino < N && distLocal[destino] < menorDist) {
            menorDist = distLocal[destino];
            melhor = destino;
        }
    }
    if (melhor != -1) {
        for (int i = 0; i < N; i++) {
            pai[i] = paiLocal[i];
            dist[i] = distLocal[i];
        }
    }
    return melhor;
}

void encerrar() {
    emitir("x", -1);
    ler();
}

int main() {
    if (scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P) != 7) exit(1);
    G = GRAPHInit(N);
    for (int i = 0; i < P; i++) {
        int p; if (scanf("%d", &p) != 1) exit(1);
        G->temPosto[p - 1] = true;
    }
    for (int i = 0; i < M; i++) {
        int u, v, w;
        if (scanf("%d %d %d", &u, &v, &w) != 3) exit(1);
        GRAPHInsert(G, EDGE(u - 1, v - 1, w));
    }
    if (scanf("%d", &Q) != 1) exit(1);
    restaurantes = malloc(Q * sizeof(Restaurante));
    if (!restaurantes) exit(1);
    for (int i = 0; i < Q; i++) {
        int r, k;
        if (scanf("%d %d", &r, &k) != 2) exit(1);
        G->temRestaurante[r - 1] = true;
        restaurantes[i].local = r - 1;
        restaurantes[i].numPedidos = k;
        restaurantes[i].destinoPedido = malloc(k * sizeof(int));
        if (!restaurantes[i].destinoPedido) exit(1);
        for (int j = 0; j < k; j++) {
            if (scanf("%d", &restaurantes[i].destinoPedido[j]) != 1) exit(1);
            restaurantes[i].destinoPedido[j]--;
        }
    }
    posicaoAtual = H - 1;
    combustivelAtual = I;

    while (1) {
        int pai[MAXN], dist[MAXN];
        Dijkstra(G, posicaoAtual, pai, dist);

        int idx = -1, bestScore = INT_MAX;
        for (int i = 0; i < Q; i++) {
            if (restaurantes[i].numPedidos == 0) continue;
            int rloc = restaurantes[i].local;
            int score = dist[rloc];
            int pdist[MAXN], ppai[MAXN];
            Dijkstra(G, rloc, ppai, pdist);
            for (int j = 0; j < restaurantes[i].numPedidos; j++) {
                int d = restaurantes[i].destinoPedido[j];
                if (d >= 0 && d < N && pdist[d] < INT_MAX) score += pdist[d];
            }
            if (score < bestScore) {
                bestScore = score;
                idx = i;
            }
        }

        if (idx == -1) break;
        int rlocal = restaurantes[idx].local;
        moverPara(rlocal, pai, dist);

        while (restaurantes[idx].numPedidos > 0 && mochilaAtual < C) {
            int melhorD = -1, melhorDist = INT_MAX;
            int distTemp[MAXN], paiTemp[MAXN];
            Dijkstra(G, rlocal, paiTemp, distTemp);
            for (int i = 0; i < restaurantes[idx].numPedidos; i++) {
                int d = restaurantes[idx].destinoPedido[i];
                if (d >= 0 && d < N && distTemp[d] < melhorDist) {
                    melhorDist = distTemp[d];
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

        int entrega;
        while (mochilaAtual > 0) {
            entrega = encontrarEntregaMaisProxima(pai, dist);
            if (entrega == -1) break;
            moverPara(entrega, pai, dist);
            entregarPedido(entrega);
        }
    }

    int pai[MAXN], dist[MAXN];
    Dijkstra(G, posicaoAtual, pai, dist);
    moverPara(H - 1, pai, dist);
    encerrar();
    return 0;
}
