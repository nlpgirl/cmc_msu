#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned g(unsigned x)
{
    unsigned d1, d2, d = 1;
    while (d*d <= x){
        if (x % d == 0){
            d1 = d;
            d2 = x / d;
            if (d1 == (int)sqrt(d2)){
               return d2;
            }
        }
        d += 1;
    }
    return 0;
}

int main(void){
unsigned x;
scanf("%u", &x);
printf("%u", g(x));
return 0;
}
