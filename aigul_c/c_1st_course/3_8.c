#include <stdio.h>
#include <stdlib.h>
int med(int a, int b, int c){
int maxim = a, minim = a;
if (b > maxim)
    maxim = b;
if (c > maxim)
    maxim = c;
if (b < minim)
    minim = b;
if (c < minim)
    minim = c;
return (a+b+c - minim - maxim);
}
int main(void){
    int n, x;
    scanf("%d", &n);
    int mas[n];
    for(int i=0; i < n; i++){
        scanf("%d", &x);
        mas[i] = x;
    }
    while (n > 0){
        n -= 2;
        for(int i=0; i < n; i++){
            mas[i] = med(mas[i], mas[i+1], mas[i+2]);
        }
    }
    printf("%d", mas[0]);

return 0;
}
