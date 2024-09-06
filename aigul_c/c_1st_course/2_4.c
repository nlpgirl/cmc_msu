#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
    int n, x, summ = 0;
    scanf("%d", &n);
    int a[n], b[n];
    for(int i=0; i < n; i++){
        scanf("%d", &x);
        a[i] = x;
    }
    for(int i=0; i < n; i++){
        scanf("%d", &x);
        b[i] = x;
    }
    for(int i=0; i < n; i++){
        summ += fmax(a[i], b[i]);
    }
    printf("%d", summ);
return 0;
}
