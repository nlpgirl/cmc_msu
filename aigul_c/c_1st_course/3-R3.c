#include <stdio.h>

int binomial_coeff(int n, int k) {
    if (k == 0 || k == n) {
        return 1;
    } else {
        return binomial_coeff(n - 1, k - 1) + binomial_coeff(n - 1, k);
    }
}

int main(void){
    int n, k;
    scanf("%d", &n);
    scanf("%d", &k);

    int bc = binomial_coeff(n, k);
    printf("%d\n", bc);
    return 0;
}
