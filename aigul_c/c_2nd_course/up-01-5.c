#include <stdio.h>
#include <stdlib.h>

void
swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void
reverse(int *arr, int start, int end)
{
    while (start < end) {
        swap(&arr[start], &arr[end]);
        start++;
        end--;
    }
}

int
next_permutation(int *arr, int n)
{
    int k;
    for (k = n - 2; k >= 0; k--) {
        if (arr[k] < arr[k + 1]) {
            break;
        }
    }

    if (k == -1) {
        return 0;
    }

    int l;
    for (l = n - 1; l > k; l--) {
        if (arr[l] > arr[k]) {
            break;
        }
    }

    swap(&arr[k], &arr[l]);
    reverse(arr, k + 1, n - 1);

    return 1; // permutation found
}

int
main(void)
{
    int n;
    if (scanf("%d", &n) == 0) {
        return 1;
    }

    int *a = (int *) malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }

    do {
        for (int i = 0; i < n; i++) {
            printf("%d", a[i]);
        }
        printf("\n");
    } while (next_permutation(a, n));

    free(a);
    return 0;
}
