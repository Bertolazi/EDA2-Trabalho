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

int moveLocal(int localAtual, int N, Graph G, int localDestino, int combustivelAtual, int *distanciaMovimento) {
    int pa[N];
    int dist[N];
    Dijkstra(G, localAtual, pa, dist);
    
    if (dist[localDestino] <= combustivelAtual) {
        *distanciaMovimento = dist[localDestino];
        return localDestino; // Retorna o novo local
    }
    return localAtual; // Permanece no mesmo local se não puder mover
}

int abastecer(int localAtual, int *postos, int *combustivelAtual, int capacidadeTanque) {
    if (postos[localAtual]) { // Se está em um posto
        *combustivelAtual = capacidadeTanque;
        return *combustivelAtual;
    }
    return 0;
}

int coletaPedido(int localAtual, Restaurante *restaurantes, int numRestaurantes, int pedidoD, int *mochila, int *numPedidosMochila, int capacidadeMochila) {
    // Verifica capacidade da mochila
    if (*numPedidosMochila >= capacidadeMochila) {
        return 0;
    }
    
    // Procura restaurante no local atual
    for (int i = 0; i < numRestaurantes; i++) {
        if (restaurantes[i].local == localAtual) {
            // Verifica se o pedido existe neste restaurante
            for (int j = 0; j < restaurantes[i].numPedidos; j++) {
                if (restaurantes[i].destinoPedido[j] == pedidoD) {
                    mochila[(*numPedidosMochila)++] = pedidoD;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int entregaPedido(int localAtual, int pedidoD, int *mochila, int *numPedidosMochila) {
    // Verifica se o local atual é o destino do pedido
    if (localAtual != pedidoD) {
        return 0;
    }
    
    // Procura pedido na mochila
    for (int i = 0; i < *numPedidosMochila; i++) {
        if (mochila[i] == pedidoD) {
            // Remove pedido (substitui pelo último elemento)
            mochila[i] = mochila[--(*numPedidosMochila)];
            return 1;
        }
    }
    return 0;
}

int *postos;                    // Array marcando locais que são postos
Restaurante *restaurantes;      // Array de restaurantes
int capacidadeCombustivel;      // T
int capacidadeMochila;          // C
int *mochila;                   // Array de pedidos coletados
int numPedidosMochila = 0;      // Contador de pedidos na mochila

int main(){
    int N, M, H, T, I, C, P, Q;
    char op;
    int X, D;
    Graph G;

    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    capacidadeCombustivel = T;
    // combustivel = I;
    capacidadeMochila = C;

    G = GRAPHInit(N+1);
    if(!G){
        return -1;
    }

    postos = malloc((N+1) * sizeof(int));
    if(!postos){
        return -1;
    }
    for(int i = 0; i < P+1; i++){
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

    mochila = malloc(capacidadeMochila * sizeof(int));
    
    int posicaoAtual = H;
    int combustivelAtual = I;
    
    while(1) {
        scanf(" %c", &op);
        int res;
        
        if (op == 'm') {
            scanf("%d", &X);
            int dist;
            int novoLocal = moveLocal(posicaoAtual, N, G, X, combustivelAtual, &dist);
            printf("%d\n", (novoLocal != posicaoAtual) ? 1 : 0);
            if (novoLocal != posicaoAtual) {
                posicaoAtual = novoLocal;
                combustivelAtual -= dist;
            }
        }
        else if (op == 'a') {
            res = abastecer(posicaoAtual, postos, &combustivelAtual, capacidadeCombustivel);
            printf("%d\n", res);
        }
        else if (op == 'p') {
            scanf("%d", &D);
            res = coletaPedido(posicaoAtual, restaurantes, Q, D, mochila, &numPedidosMochila, capacidadeMochila);
            printf("%d\n", res);
        }
        else if (op == 'o') {
            scanf("%d", &D);
            res = entregaPedido(posicaoAtual, D, mochila, &numPedidosMochila);
            printf("%d\n", res);
        }
        else if (op == 'x') {
            printf("1\n");
            break;
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
