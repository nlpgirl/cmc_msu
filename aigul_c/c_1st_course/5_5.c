#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    int L;
    scanf("%d", &L);
    char **a = (char**) malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        a[i] = (char*) malloc(L * sizeof(char));
    }
    for (int i = 0; i < 3; i++) {
        scanf("%s", a[i]);
    }
    printf("%s%s%s", a[2], a[0], a[1]);
    for (int i = 0; i < 3; i++) {
        free(a[i]);
    }
    free(a);
}

