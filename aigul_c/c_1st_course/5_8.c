#include <stdio.h>
#include <stdlib.h>

int main(void){
    int n, xn, yn, xk, yk;
    scanf("%d", &n);
    scanf("%d %d %d %d", &xn, &yn, &xk, &yk);
    int **a = NULL;
    a = (int**)malloc(sizeof(int*)*n); //выделяем память под двумерный массив
    for(int i=0; i<n; i++){
        a[i] = (int*)malloc(sizeof(int)*n);
    }
    for(int i=0; i<n; i++){ //обнуляем все элементы массива а
        for(int j=0; j<n; j++){
            a[i][j] = 0;
        }
    }
    int xshift[8] = {1, -1, -2, -2, -1, 1, 2, 2};
    int yshift[8] = {-2, -2, -1, 1, 2, 2, 1, -1};
    int *x, *y;
    x = (int*) malloc(8*sizeof(int));
    y = (int*) malloc(8*sizeof(int));
    x[0] = xn; y[0] = yn;
    //x[0] = xn; y[0] = yn;
    int step = 0;
    int previousamount = 1, newamount = 0, all=1, xt, yt;

    while (all <= n*n){
        step += 1;
        for(int i=0; i<previousamount; i++){
            for(int j=0; j<8; j++){
                xt = x[i] + xshift[j];
                yt = y[i] + yshift[j];
                if(xt >= 1 && xt <= n && yt >= 1 && yt <= n && a[xt][yt] == 0){
                    a[xt][yt] = step;
                    x[newamount+previousamount] = xt;
                    y[newamount+previousamount] = yt;
                    newamount++;
                }
            }
        }
        realloc(x, (previousamount+8*newamount)*sizeof(int));
        realloc(y, (previousamount+8*newamount)*sizeof(int));
        all += newamount;
        x = x + previousamount;
        y = y + previousamount;
        previousamount = newamount;
        newamount = 0;
    }
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ", a[i][j]);
        }
    printf("\n");
    }

    return 0;
}
