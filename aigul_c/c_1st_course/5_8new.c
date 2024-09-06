#include <stdio.h>
#include <stdlib.h>

int main(void){
    int n, xn, yn, xk, yk;
    scanf("%d", &n);
    scanf("%d %d %d %d", &xn, &yn, &xk, &yk);
    xn--; yn--; xk--; yk--;
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

    int *xprevious, *yprevious, *xnew, *ynew;
    xprevious = (int*) malloc(sizeof(int));
    yprevious = (int*) malloc(sizeof(int));
    xnew = (int*) malloc(sizeof(int)*8);
    ynew = (int*) malloc(sizeof(int)*8);
    xprevious[0] = xn; yprevious[0] = yn;

    int step = 0;
    int previousamount = 1, newamount = 0, all=1, xt, yt;

    while (step <= 5){
        step += 1;
        for(int i=0; i<previousamount; i++){
            for(int j=0; j<8; j++){
                xt = xprevious[i] + xshift[j];
                yt = yprevious[i] + yshift[j];
                if(xt >= 1 && xt <= n && yt >= 1 && yt <= n && a[xt][yt] == 0){
                    a[xt][yt] = step;
                    xnew[newamount] = xt;
                    ynew[newamount] = yt;
                    newamount++;
                }
            }
        }
        free(xprevious);
        free(yprevious);
        xprevious = malloc(newamount*sizeof(int));
        yprevious = malloc(newamount*sizeof(int));
        for(int i=0; i<newamount; i++){
            xprevious[i] = xnew[i];
            yprevious[i] = ynew[i];
        }
        free(xnew);
        free(ynew);
        xnew = malloc(sizeof(int)*8*newamount);
        ynew = malloc(sizeof(int)*8*newamount);
        all += newamount;
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
