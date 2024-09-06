#include <stdlib.h>

static int
comp(const void *a, const void *b)
{
    int l = *(int *) a;
    int r = *(int *) b;
    int tmp = abs(l) % 2 - abs(r) % 2;
    if (tmp == 0) {
        tmp = l - r;
        if (l % 2 == 0) {
            return tmp;
        } else {
            return -tmp;
        }
    }
    return tmp;
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(data[0]), comp);
}
