#include <stdio.h>
#include <stdlib.h>

int main(){
    int N, M, H, T, I, C, P, Q;
    char op;
    int X, D;
    scanf("%d %d %d %d %d %d %d", &N, &M, &H, &T, &I, &C, &P);
    int o[P], u[M], v[M], w[M];
    for(int i = 0; i < P; i++){
        scanf("%d", &o[i]);
    }
    for(int j = 0; j < M; j++){
        scanf("%d %d %d", &u[j], &v[j], &w[j]);
    }
    scanf("%d", &Q);
    int r[Q], k[Q];
    int** d = (int**)malloc(Q * sizeof(int*));
    for(int m = 0; m < Q; m++){
        scanf("%d", &r[m], &k[m]);
        d[m] = (int*)malloc(k[m] * sizeof(int)); 
        for(int n = 0; n < k[m]; n++){
            scanf("%d", &d[m][n]);
    }
    scanf("%c", &op);
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
    return 0;
}