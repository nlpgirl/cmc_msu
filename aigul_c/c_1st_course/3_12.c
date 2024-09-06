#include <stdio.h>
int main(void) {
    int n, x;
    scanf("%d", &n);
    int a[n];
    for (int i = 0; i < n; i++){
        scanf("%d", &x);
        a[i] = x;
    }
    int s1 = 0, s2 = 0, maxs = 0;
    for (int mask1 = 0; mask1 < (1<<n); mask1++){
        for (int mask2 = 0; mask2 < (1<<n); mask2++){ //mask2 = mask1+1;
            s1 = 0;
            s2 = 0;
            if((mask1 & mask2) == 0){
                for(int bit = 0; bit < n; bit++){
                    if ((mask1>>bit)&1) s1 += a[bit]; //передвигаем каждый бит в конец и побитовая конъюкция с единицей говорит нам, нужно нам брать
                    if ((mask2>>bit)&1) s2 += a[bit]; //соответстсвующий элемент или не нужно
                }
                if(s1 == s2){
                    if (s1>maxs) maxs = s1;
                }
            }
        }
    }
    printf("%d", maxs);
    return 0;
}
