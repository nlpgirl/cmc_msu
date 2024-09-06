#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "roots.h"
int iter = 0;
double root(double(*f)(double), double(*g)(double), double a, double b, double eps1) {
    double a_func, c_func, c;
    do {
        iter++;
        c = (a+b)/2;
        a_func = f(a) - g(a);
        c_func = f(c) - g(c);
        if(a_func*c_func < 0) b = c;
        else a = c;
    }
    while(fabs(c_func) >= eps1);
    return c;
}
