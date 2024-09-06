#include <stdio.h>
#include <stdlib.h>
#include <math.h>
unsigned long long sqroot(unsigned long long x) {
    unsigned long long d = 1;
    while (d * d <= x) {
        d += 1;
    }
    return d - 1;
}

unsigned long long g(unsigned long long x) {
    unsigned long long d1, d2, d = 1;
    while (d * d <= x) {
        if (x % d == 0) {
            d1 = d;
            d2 = x / d;
            if (d1 == sqroot(d2)) {
               return d2%(long int)pow(2,32);
            }
        }
        d += 1;
    }
    return 0;
}

int main(void) {
    unsigned long long x;
    scanf("%llu", &x);
    printf("%llu", g(x));
    return 0;
}
