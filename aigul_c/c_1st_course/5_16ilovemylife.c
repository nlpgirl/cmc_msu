#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void){
    int k, n, p, x;
    scanf("%d", &k); scanf("%d", &n); scanf("%d", &p);
    int f[k], fprevious[k];

    //динамически выделим память для массива a[k][k]
    int **a = (int**)malloc(k*sizeof(int*));
    for(int i=0; i<k; i++){
        a[i] = (int*)malloc(k*sizeof(int));
    }

    for(int i=0; i<k; i++){
        scanf("%d", &x);
        f[i] = x%p;
    }

    for(int i=0; i<k; i++){
        scanf("%d", &x);
        a[0][i] = x%p;
    }
    //создадим перевернутую версию массива f
    for(int i=0; i<k; i++){
        fprevious[k-i-1] = f[i];
    }
    for(int i=1; i<k; i++){
        for(int j=0; j<k; j++){
            a[i][j] = 0;
        }
        a[i][i-1] = 1;
    }
    printf("raspechataem vse k chertyam\n");
    printf("fprevious\n");
    for(int i=0; i<k; i++){
        printf("%d ", fprevious[i]);
    }
    printf("\na\n");
    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
    int fnew[k];
    for(int z=0; z<n-k; z++){ //нужно n-k раз посчитать матрицу fnew, чтобы добраться до ответа
        for(int i=0; i<k; i++){//хотим посчитать матрицу fnew размера k*1
            fnew[i] = 0;
            for(int j=0; j<k; j++){
                fnew[i] += (a[i][j] * fprevious[j])%p;

            }
            printf("%d ", fnew[i]);
        }
        printf("\n");
        memmove(fprevious, fnew, k*sizeof(int));
    }
    printf("answer is %d", fnew[0]%p);


    return 0;
}
