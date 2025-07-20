#include <stdio.h>
#include <stdlib.h>

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

int moveLocal(int local){
    return local;
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
int capacidadeCombustivel, combustivel,
    capacidadeMochila;
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

    // postos
    int posto = -1;
    for(int i = 0; i < P; i++){
        scanf("%d", &posto);
        postos[posto] = 1;
    }
    int u, v, w;
    //ruas
    for(int j = 0; j < M; j++){
        scanf("%d %d %d", &u, &v, &w);
        GRAPHInsert(G, EDGE(u, v, w));
    }

    // qtde restaurantes
    scanf("%d", &Q);
    restaurantes = malloc(Q * sizeof(Restaurante));
    if(!restaurantes){
        return -1;
    }
    // dados resturante
    int r, k;
    // local restaurante, numero pedidos, destino pedidos
    for(int m = 0; m < Q; m++){
        scanf("%d %d", &r, &k);
        restaurantes[m].local = r;
        restaurantes[m].numPedidos = k;
        restaurantes[m].destinoPedido = malloc(k*sizeof(int));
        for(int n = 0; n < k; n++)
            scanf("%d", &restaurantes[m].destinoPedido[n]);
    }
    
    while(1){
        // Menu de opções
        scanf(" %c", &op);
        int res;
        switch (op)
        {
            case 'm':
                scanf("%d", &X);
                // Função de mover para X
                res = moveLocal(X);
                printf("%d", res);
                break;
            case 'a':
                // Função de abastecer
                res = abastecer();
                printf("%d", res);
                break;
            case 'p':
                scanf("%d", &D);
                // Função pegar pedido D
                res = coletaPedido(D);
                printf("%d", res);
                break;
            case 'o':
                scanf("%d", &D);
                // Função entregar pedido no local D
                res = entregaPedido(D);
                printf("%d", res);
                break;
            case 'x':
                // Função de encerrar a entrega
                printf("1 \n");
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