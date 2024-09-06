#include <stdio.h>
#include <stdlib.h>

void f(int cnt) {
    char * s;
    s = (char *) malloc(100 * sizeof(char));

    if (scanf("%s", s) != 1) {
        return;
    }

    if (cnt % 2 == 0) {
        printf("%s ", s);
        f(++cnt);
    } else {
        f(++cnt);
        printf("%s ", s);
    }
}


int main()
{
    f(1);

    return 0;
}
