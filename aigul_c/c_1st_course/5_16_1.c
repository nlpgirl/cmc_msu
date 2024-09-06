#include <stdio.h>
#include <stdlib.h>

void multiply(int **a, int **b, int k, int p){
    int **product = (int**)malloc(k*sizeof(int*));
    for(int i=0; i<k; i++){
        product[i] = (int*)calloc(k, k*sizeof(int));
    }
    for(int i=0; i<k; i++){
        for(int j=0; j<k; j++){
            for(int n=0; n<k; n++){
                product[i][j] += (a[i][n]*b[n][j])%p;
            }
        }
    }
    for (int i = 0; i < k; i++) {
        for(int j=0; j < k; j++){
            a[i][j] = product[i][j];
        }
    }
    for (int i = 0; i < k; i++) {
        free(product[i]);
    }
    free(product);
    //return a;
}

int main(void){
    int k, n, p, x;
    scanf("%d", &k); scanf("%d", &n); scanf("%d", &p);
    int f[k], fordered[k];

    int **a = (int**)malloc(k*sizeof(int*));
    for(int i=0; i<k; i++){
        a[i] = (int*)calloc(k, k*sizeof(int));
    }

    for(int i=0; i<k; i++){
        scanf("%d", &x);
        fordered[i] = x%p;
    }

    for(int i=0; i<k; i++){
        scanf("%d", &x);
        a[0][i] = x%p;
    }
    if(n<k){
        printf("%d", fordered[n-1]%p);
        return 0;
    }
    for(int i=0; i<k; i++){
        f[k-i-1] = fordered[i];
    }
    for(int i=1; i<k; i++){
        a[i][i-1] = 1;
    }
    int power = n-1;
    int **res = (int**)malloc(k*sizeof(int*));
    for(int i=0; i<k; i++){
        res[i] = (int*)calloc(k, k*sizeof(int));
    }
    for(int i=0; i<k; i++){
        res[i][i] = 1;
    }

    while(power){
        if(power%2){
            multiply(res, a, k, p);
            power--;
        }
        else{
            multiply(a, a, k, p);
            power /= 2;
        }

    }

    int answer[k];
    for(int i=0; i<k; i++){
            answer[i] = 0;
            for(int j=0; j<k; j++){
                answer[i] += (res[i][j] * f[j])%p;
            }
        }
    printf("%d", answer[k-1]%p);

    return 0;
}
