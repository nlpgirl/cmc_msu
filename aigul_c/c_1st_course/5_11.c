#include <stdio.h>
#include <stdlib.h>
int max2(int x, int y){
    if(x > y) return x;
    return y;
}
int max3(int x, int y, int z){
    if(x >= y && x >= z) return x;
    if(y >= x && y >= z) return y;
    if(z >= x && z >= y) return z;

}
int max(int **a, int i, int j, int n){
    if(i==0) return max2(a[0][j-1], a[1][j-1]);
    if(i==n-1) return max2(a[i][j-1], a[i-1][j-1]);
    return max3(a[i-1][j-1], a[i][j-1], a[i+1][j-1]);
}

int main(void){
    int n, m;
    scanf("%d %d", &n, &m);
    //создадим двумерный массив а
    int **a;
    a = (int**)malloc(n*sizeof(int*));

    for(int i=0; i<n; i++){
        a[i] = (int*)malloc(m*sizeof(int));
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            scanf("%d", &a[i][j]);
        }
    }
    for(int j=1; j<m; j++){
        for(int i=0; i<n; i++){
            a[i][j] += max(a, i, j, n);
        }
    }
    int ans = a[0][m-1];
    for(int i=1; i<n; i++){
        if(a[i][m-1] > ans) ans = a[i][m-1];
    }
    printf("%d", ans);

    for(int i=0; i<n; i++){
        free(a[i]);
    }
    free(a);
    return 0;
}
