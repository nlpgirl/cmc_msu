#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(void){
    int k, n, p, x;
    int newelement = 0;
    scanf("%d", &k); scanf("%d", &n); scanf("%d", &p);
    int *f;
    int a[k+1];
    f = malloc(k*sizeof(int));
    for(int i=1; i<k; i++){
        scanf("%d", &x);
        f[i] = x%p;
    }
    scanf("%d", f[0]);
    for(int i=1; i<=k; i++){
        scanf("%d", &x);
        a[i] = x%p;
    }

    for(int i=k+1; i<=n; i++){
        newelement = 0;
        for(int j=1; j<=k; j++){
            printf("J");
            newelement += (a[j]*f[(i-j)%k])%p;

        }
        printf("K");
        //printf("%d\n", newelement);

        f[i%k] = newelement%p;
        printf("K2");
        printf("i=%d ", i);
        for(int z=1; z<=k; z++){
            printf("f[%d]=%d ",z , f[z]);
        }
        printf("\n");


    }
    printf("%d", f[0]);

    return 0;
}
