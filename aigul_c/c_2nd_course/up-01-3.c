#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <stdlib.h>

long long
powerFast(long long num, int deg, int n)
{
    long long result = 1;
    while (deg) {
        if (deg % 2 == 0) {
            deg /= 2;
            num = (num * num) % n;
        } else {
            --deg;
            result = (result * num) % n;
        }
    }
    return result % n;
}

int
find_quotient(int a, int c, int n)
{
    long long inv_a = 1;
    double check;
    if (a != 0) {
        check = log(LLONG_MAX) / log(a);
    } else {
        exit(1);
    }
    if (n < check + 2) {
        inv_a = pow(a, n - 2) % n;
    } else {
        inv_a = powerFast(a, n - 2, n) % n;
    }
    return (c * inv_a) % n;
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) < 1) {
        return 1;
    }
    for (int c = 0; c < n; ++c) {
        for (int a = 1; a < n; ++a) {
            printf("%d ", find_quotient(a, c, n));
        }
        printf("\n");
    }
    return 0;
}
