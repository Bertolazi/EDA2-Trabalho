#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct node *link;
struct node{int w, custo; link next;};

typedef struct Graph *Graph;
struct Graph {
    int v, e;
    link *adj;
};

typedef struct{
    int local, numPedidos, *destinoPedido;
}Restaurante;

typedef struct {
    int v, w, custo;
} Edge;

Edge EDGE(int v, int w, int custo) {
    Edge e;
    e.v = v;
    e.w = w;
    e.custo = custo;
    return e;
}

link New(int w, int custo, link next){
    link x = malloc(sizeof(*x));
    if(x==NULL) return NULL;
    x->w = w; 
    x->custo = custo;
    x->next = next;
    return x;
}

Graph GRAPHInit(int v){
    Graph g = malloc(sizeof(*g));
    g->v = v;
    g->e=0;
    g->adj = malloc(v*sizeof(link));
    for(int i=0; i < v; i++){
        g->adj[i]=NULL;
    }
    return g;
}

void GRAPHInsert(Graph g, Edge e){
    int v=e.v, w=e.w, custo=e.custo;
    g->adj[v] = New(w, custo, g->adj[v]);
    g->e++;
}

void Dijkstra(Graph G, int s, int *pa, int *distD){
    bool mature[1000];
    for(int v = 0; v < G->v; v++) {
        pa[v] = -1;
        mature[v] = false;
        distD[v] = INT_MAX;
    }
    distD[s] = 0;
    while(true){
        int min = INT_MAX;
        int y = -1;
        for(int z = 0; z < G->v; z++){
            if(!mature[z] && distD[z] < min){
                min = distD[z];
                y = z;
            }
        }
        if(y == -1)
            break;
        for(link a = G->adj[y]; a != NULL; a = a->next){
            int w = a->w;
            int custo = a->custo;
            if(!mature[w] && distD[y] + custo < distD[w]){
                distD[w] = distD[y] + custo;
                pa[w] = y;
            }
        }
        mature[y] = true;
    }
}

int moveLocal(int local, int N, Graph G, int localDestino, int combustivel,int *distM){
    int pa[N];
    int distMInterno[N];
    Dijkstra(G, local, pa, distMInterno);
    if(distMInterno[localDestino] <= combustivel){
        *distM = distMInterno[localDestino];
        return localDestino;
    }
    return 0;
}

int abastecer(){
    return 1;
}

int coletaPedido(int pedido){
    return pedido;
}

int entregaPedido(int pedido){
    return pedido;
}

int *postos;
Restaurante *restaurantes;
int capacidadeCombustivel, combustivel, capacidadeMochila;
link ultimoLocal;

int main(){
    int N, M, H, T, I, C, P, Q;
    char op;
    int X, D;
    Graph G;

    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    capacidadeCombustivel = T;
    combustivel = I;
    capacidadeMochila = C;

    G = GRAPHInit(N);
    if(!G){
        return -1;
    }

    postos = malloc(P * sizeof(int));
    if(!postos){
        return -1;
    }
    for(int i = 0; i < P; i++){
        postos[i] = 0;
    }

    int posto = -1;
    for(int i = 0; i < P; i++){
        scanf("%d", &posto);
        postos[posto] = 1;
    }

    int u, v, w;
    for(int j = 0; j < M; j++){
        scanf("%d %d %d", &u, &v, &w);
        GRAPHInsert(G, EDGE(u, v, w));
    }

    scanf("%d", &Q);
    restaurantes = malloc(Q * sizeof(Restaurante));
    if(!restaurantes){
        return -1;
    }

    int r, k;
    for(int m = 0; m < Q; m++){
        scanf("%d %d", &r, &k);
        restaurantes[m].local = r;
        restaurantes[m].numPedidos = k;
        restaurantes[m].destinoPedido = malloc(k*sizeof(int));
        for(int n = 0; n < k; n++)
            scanf("%d", &restaurantes[m].destinoPedido[n]);
    }

    int posicaoAtual = H, combustivelAtual = I;
    while(1){
        scanf(" %c", &op);
        int res;
        switch (op)
        {
            case 'm':
                scanf("%d", &X);
                int distMov;
                res = moveLocal(posicaoAtual, N, G, X, combustivelAtual, &distMov);
                printf("%d\n", res);
                if(res != 0){
                    combustivelAtual -= distMov;
                    posicaoAtual = X;
                }
                break;
            case 'a':
                res = abastecer();
                printf("%d", res);
                break;
            case 'p':
                scanf("%d", &D);
                res = coletaPedido(D);
                printf("%d", res);
                break;
            case 'o':
                scanf("%d", &D);
                res = entregaPedido(D);
                printf("%d", res);
                break;
            case 'x':
                printf("1\n");
                return 0;
        }
    }

    for (int i = 0; i < N; i++) {
        link atual = G->adj[i];
        while (atual != NULL) {
            link proximo = atual->next;
            free(atual);
            atual = proximo;
        }
    }
    free(G->adj);
    free(G);   
    free(postos);
    for (int m = 0; m < Q; m++)
        free(restaurantes[m].destinoPedido);
    free(restaurantes);
    return 0;
}
