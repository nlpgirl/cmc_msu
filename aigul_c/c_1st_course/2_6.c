#include <stdio.h>
int main(void){
    int n, x;
    scanf("%d", &n);
    int p[n+1];
    for (int i = 1; i <= n; i++){
        scanf("%d", &x);
        p[i] = x;
    }
    for (int i = 1; i <= n; i++){
        printf("%d ", p[p[p[i]]]);
    }

return 0;
}
