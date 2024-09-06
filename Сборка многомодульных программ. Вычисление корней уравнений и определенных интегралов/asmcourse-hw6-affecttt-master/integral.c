#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "integral.h"

double integral(double(*f)(double), double a, double b, double eps2) {
    int k, n = 10;
    double h = (b-a)/n; 
    double prev = 0;
    double now = f(a) + f(b);
    while(fabs(prev - now) >= eps2){
        now = f(a) + f(b);
        h = (b-a)/n;
        for(int i = 1; i <= n - 1; i++){
            k = 2 + 2 * (i % 2);
            now += k * f(a + i*h);
        }
        prev = now;
        n += 10;
    }
    now *= h / 3;
    return now;
}
