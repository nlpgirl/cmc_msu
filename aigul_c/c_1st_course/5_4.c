#include <stdio.h>
#include <stdlib.h>

int main(void){
    int count = 0, n = 0, x;
    int *a = NULL;
    scanf("%d", &x);
    while(x != 0){
        if(count == n){
            if(n == 0) n = 1;
            else n *= 2;
        }
        a = (int *)realloc(a, sizeof(int)*n);
        a[count] = x;
        count++;
        scanf("%d", &x);
    }
    int ans = 0;
    for(int i=0; i<count; i++){
        if(a[i] < a[count-1]){
            ans++;
        }
    }
    printf("%d", ans);
    free(a);
    return 0;
}
