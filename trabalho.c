#include <stdio.h>
#include <stdlib.h>

int main(){
    int N, M, H, T, I, C, P, Q;
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
    return 0;
}