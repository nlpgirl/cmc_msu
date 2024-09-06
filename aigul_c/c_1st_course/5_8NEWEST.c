#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    int n, xn, yn, xk, yk, flag = 1;
    scanf("%d", &n);
    scanf("%d %d %d %d", &xn, &yn, &xk, &yk);
    int **a = NULL;
    a = (int**)malloc(sizeof(int*)*(n+1)); //выделяем память под двумерный массив
    for(int i=0; i<=n; i++){
        a[i] = (int*)malloc(sizeof(int)*(n+1));
    }
    for(int i=0; i<=n; i++){ //обнуляем все элементы массива а
        for(int j=0; j<=n; j++){
            a[i][j] = 0;
        }
    }
    int previousamount = 1, newamount = 0, currentx, currenty, step=0;
    int *xprevious, *yprevious, *xnew, *ynew;
    xprevious = malloc(sizeof(int)); yprevious = malloc(sizeof(int));
    xnew = malloc(sizeof(int)*8); ynew = malloc(sizeof(int)*8);
    xprevious[0] = xn; yprevious[0] = yn;
    //int xprevious[1] = {xn}, yprevious[1] = {yn};
    //int xnew[8], ynew[8];
    int xshift[8] = {1, -1, -2, -2, -1, 1, 2, 2};
    int yshift[8] = {-2, -2, -1, 1, 2, 2, 1, -1};
    while(flag){
            printf("xprevious and yprevious\n");
            for(int j=0; j<previousamount; j++){
                printf("%d ", xprevious[j]);
            }
            printf("\n");
            for(int j=0; j<previousamount; j++){
                printf("%d ", yprevious[j]);
            }
            printf("\n");
            step++;
            for(int i=0; i<previousamount; i++){
                for(int j=0; j<8; j++){
                    currentx = xprevious[i] + xshift[j];
                    currenty = yprevious[i] + yshift[j];
                    if(currentx >= 1 && currentx <= n && currenty >= 1 && currenty <= n && a[currentx][currenty] == 0){
                        a[currentx][currenty] = step;
                        xnew[newamount] = currentx;
                        ynew[newamount] = currenty;
                        newamount++;
                        printf("%d %d step=%d\n", currentx, currenty, step);
                    }
                }
            }
            printf("xnew and ynew\n");
            for(int j=0; j<newamount; j++){
                printf("%d ", xnew[j]);
            }
            printf("\n");
            for(int j=0; j<newamount; j++){
                printf("%d ", ynew[j]);
            }
            printf("\n");
            if(newamount == 0) flag = 0;
            realloc(xprevious, sizeof(int)*newamount);
            realloc(yprevious, sizeof(int)*newamount);
            memmove(xprevious, xnew, sizeof(int)*newamount);
            memmove(yprevious, ynew, sizeof(int)*newamount);
            realloc(xnew, sizeof(int)*newamount*8);
            realloc(ynew, sizeof(int)*newamount*8);
            previousamount = newamount;
            newamount = 0;
    }
    printf("%d", a[xk][yk]);
    return 0;
}
