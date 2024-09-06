#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "test.h"

double f1test(double x) {
    return -x*x*x*x + 16;
}

double f2test(double x) {
    return x*x*x*x + 14;
}

double f3test(double x) {
    return -5*x + 10;
}