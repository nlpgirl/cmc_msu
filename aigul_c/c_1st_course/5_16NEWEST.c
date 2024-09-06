#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void){
    int k, n, p, x, newelement = 0;
    scanf("%d", &k); scanf("%d", &n); scanf("%d", &p);
    int *f, a[k+1];
    f = malloc(k*sizeof(int));
    for(int i=1; i<=k; i++){
        scanf("%d", &x);
        f[i] = x%p;
    }
    for(int i=1; i<=k; i++){
        scanf("%d", &x);
        a[i] = x%p;
    }

    printf("%d", f[k]);
    free(f);
    return 0;
}
