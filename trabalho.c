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

int moveLocal(int local, Graph G, int localDestino, int combustivelDisponivel, int *distM){
    for(link a = G->adj[local]; a != NULL; a = a->next){
        if(a->w == localDestino){
            if(a->custo <= combustivelDisponivel){
                *distM = a->custo;
                return 1;  
            } else {
                return 0;  
            }
        }
    }
    return 0; 
}

int abastecer(int posicao, int *postinho, int capacidadeMaxima){
    if(postinho[posicao] == 1)
        return capacidadeMaxima;
    else
        return 0;
}


int coletaPedido(int localAgora, Restaurante *restaurantes, int mochilaMax, int *mochilaAgora, int Q, int *mochila){
    if (*mochilaAgora >= mochilaMax)
        return -1;
    for (int i = 0; i < Q; i++) {
        if (restaurantes[i].local == localAgora && restaurantes[i].numPedidos > 0) {
            int destino = restaurantes[i].destinoPedido[0];
            for (int j = 1; j < restaurantes[i].numPedidos; j++)
                restaurantes[i].destinoPedido[j - 1] = restaurantes[i].destinoPedido[j];
            restaurantes[i].numPedidos--;
            mochila[*mochilaAgora] = destino;
            (*mochilaAgora)++;
            return 1;  
        }
    }
    return -1;  
}


int entregaPedido(int posicaoAtual, int destino, int *mochila, int *mochilaAtual) {
    if (posicaoAtual != destino)
        return 0;
    for (int i = 0; i < *mochilaAtual; i++) {
        if (mochila[i] == destino) {
            for (int j = i + 1; j < *mochilaAtual; j++) {
                mochila[j - 1] = mochila[j];
            }
            (*mochilaAtual)--;  
            return 1;  
        }
    }
    return 0;  
}



int *postos;
Restaurante *restaurantes;
int Cc, Co, Cm;
link ultimoLocal;


int main(){
    int N, M, H, T, I, C, P, Q;
    char op;
    int X, D;
    Graph G;

    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    Cc = T;
    Co = I;
    Cm = C;

    G = GRAPHInit(N);

    postos = calloc(N + 1, sizeof(int)); 
    if(!postos) return -1;

    for(int i = 0; i < P; i++){
        int posto;
        scanf("%d", &posto);
        postos[posto - 1] = 1; 
    }

    for(int j = 0; j < M; j++){
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);
        GRAPHInsert(G, EDGE(u - 1, v - 1, w)); 
    }

    scanf("%d", &Q);
    restaurantes = malloc(Q * sizeof(Restaurante));
    if(!restaurantes) return -1;

    for(int m = 0; m < Q; m++){
        int r, k;
        scanf("%d %d", &r, &k);
        restaurantes[m].local = r - 1; 
        restaurantes[m].numPedidos = k;
        restaurantes[m].destinoPedido = malloc(k * sizeof(int));
        for(int n = 0; n < k; n++){
            scanf("%d", &restaurantes[m].destinoPedido[n]);
            restaurantes[m].destinoPedido[n]--; 
        }
    }

    int posicaoAtual = H - 1, combustivelAtual = I, mochilaAtual = 0;
    int *mochila = malloc(Cm * sizeof(int));
    for (int i = 0; i < N; i++) {
    while(1){
        scanf(" %c", &op);
        int res, col, des;
        switch (op){
            case 'm':
                scanf("%d", &X);
                X--; 
                int distMov;
                res = moveLocal(posicaoAtual, G, X, combustivelAtual, &distMov);
                if(res != 0){
                    combustivelAtual -= distMov;
                    posicaoAtual = X;
                    printf("1\n");
                } else {
                    printf("0\n");
                }
                break;
            case 'a':
                if (combustivelAtual == Cc) {
                    printf("0\n");
                    break;
                }
                if (postos[posicaoAtual] == 1) {
                    combustivelAtual = Cc;
                    printf("%d\n", Cc);
                } else {
                    printf("0\n");
                }
                break;

            case 'p':
                scanf("%d", &D);
                D--; 
                col = coletaPedido(posicaoAtual, restaurantes, Cm, &mochilaAtual, Q, mochila);
                printf("%d\n", col);
                break;

            case 'o':
                scanf("%d", &D);
                D--; 
                des = entregaPedido(posicaoAtual, D, mochila, &mochilaAtual);
                printf("%d\n", des);
                break;

            case 'x':
                printf("1\n");
                fflush(stdout); 
                return 0;
            }
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
    free (mochila);
    free(G->adj);
    free(G);   
    free(postos);
    for (int m = 0; m < Q; m++)
        free(restaurantes[m].destinoPedido);
    free(restaurantes);
    return 0;
}
