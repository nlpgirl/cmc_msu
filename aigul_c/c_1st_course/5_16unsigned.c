#include <stdio.h>
#include <stdlib.h>

unsigned **multiply(unsigned **a, unsigned **b, unsigned k, unsigned p){
    unsigned **product = (unsigned**)malloc(k*sizeof(unsigned*));
    for(unsigned i=0; i<k; i++){
        product[i] = (unsigned*)calloc(k, k*sizeof(unsigned));
    }
    for(unsigned i=0; i<k; i++){
        for(unsigned j=0; j<k; j++){
            for(unsigned n=0; n<k; n++){
                product[i][j] += (a[i][n]*b[n][j])%p;
            }
        }
    }
    /*
    for (int i = 0; i < k; i++) {
        free(a[i]);
        free(b[i]);
    }
    free(a);
    free(b);
    */
    return product;
}

int main(void){
    unsigned k, n, p, x;
    scanf("%d", &k); scanf("%d", &n); scanf("%d", &p);
    unsigned f[k], fordered[k];

    unsigned **a = (unsigned**)malloc(k*sizeof(unsigned*));
    for(unsigned i=0; i<k; i++){
        a[i] = (unsigned*)calloc(k, k*sizeof(unsigned));
    }

    for(unsigned i=0; i<k; i++){
        scanf("%u", &x);
        fordered[i] = x%p;
    }

    for(unsigned i=0; i<k; i++){
        scanf("%u", &x);
        a[0][i] = x%p;
    }
    if(n<=k){
        printf("%u", fordered[n-1]%p);
        return 0;
    }
    for(unsigned i=0; i<k; i++){
        f[k-i-1] = fordered[i];
    }
    for(unsigned i=1; i<k; i++){
        a[i][i-1] = 1;
    }
    unsigned power = n-k;
    unsigned **res = (unsigned**)malloc(k*sizeof(unsigned*));
    for(unsigned i=0; i<k; i++){
        res[i] = (unsigned*)calloc(k, k*sizeof(unsigned));
    }
    for(unsigned i=0; i<k; i++){
        res[i][i] = 1;
    }

    while(power){
        if(power%2){
            res = multiply(res, a, k, p);
        }
        a = multiply(a, a, k, p);
        power /= 2;
    }

    unsigned answer[k];
    for(unsigned i=0; i<k; i++){
            answer[i] = 0;
            for(unsigned j=0; j<k; j++){
                answer[i] += (res[i][j] * f[j])%p;
            }
        }
    printf("%u", answer[0]%p);

    return 0;
}
