#include <stdio.h>
#include <stdlib.h>

typedef struct node *link;
struct node{int v, custo; link next;};

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

link New(int v, int custo, link next){
    link x = malloc(sizeof(*x));

    if(x==NULL) return NULL;

    x->v = v; 
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

int *postos;
Restaurante *restaurantes;

int main(){
    int N, M, H, T, I, C, P, Q;
    char op;
    int X, D;
    Graph G;

    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    
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
    
    // Menu de opções
    scanf(" %c", &op);
    
    if(op == 'm')
    {
        scanf("%d", &X);
        // Função de mover para X
    }
    else if (op == 'a')
    {
        // Função de abastecer
    }
    else if(op == 'p')
    {
        scanf("%d", &D);
        // Função pegar pedido D
    }
    else if (op == 'o')
    {
        scanf("%d", &D);
        // Função entregar pedido no local D
    }
    else if(op == 'x')
    {
        // Função de encerrar a entrega
    }

    free(G);
    free(postos);
    free(restaurantes);
    return 0;
}