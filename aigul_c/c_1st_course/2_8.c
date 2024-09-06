#include <stdio.h>
int main(void){
    int n, x, k, count = 0;
    scanf("%d", &n);
    int mas[n+1];
    for (int i=1; i <= n; i++){
        scanf("%d", &x);
        mas[i] = x;
    }
    for (int i=1; i <= n-2; i++){
            for (int j=i+2; j <= n; j++){
                k = (i+j)/2;
                if (((i+j)%2 == 0) && ((mas[i]+mas[j])%2 == 0) && (mas[k] == ((mas[i]+mas[j])/2))){
                    count += 1;
                }
                //((i+j)%2 == 0) && ((mas[i]+mas[j])%2 == 0)&&

        }
    }
    printf("%d", count);
return 0;
}
