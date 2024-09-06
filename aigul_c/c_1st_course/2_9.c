#include <stdio.h>
#include <math.h>

int main(void){
    int n, x, m;
    scanf("%d", &n);
    int a[n];
    for (int i=0; i < n; i++){
        scanf("%d", &x);
        a[i] = x;
    }
    int ans = 0;
    //сейчас считаем след массив
    scanf("%d", &m);
    int b[m];
    for (int i=0; i < m; i++){
        scanf("%d", &x);
        b[i] = x;
    }
    for (int j=m-1; j >= 0; j--){
        for (int i=0; i < n; i++){
            ans += a[i]*pow(b[j], i);
        }
        printf("%d ", ans);
        ans = 0;
    }
return 0;
}
