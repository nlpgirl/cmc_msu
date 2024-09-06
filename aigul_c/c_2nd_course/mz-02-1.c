#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    char *s1 = (char *) malloc(129 * sizeof(char));
    char *s2 = (char *) malloc(129 * sizeof(char));
    char *s3 = (char *) malloc(129 * sizeof(char));
    if (scanf("%s", s1) == 0) {
        return 1;
    }
    if (scanf("%s", s2) == 0) {
        return 2;
    }
    if (scanf("%s", s3) == 0) {
        return 3;
    }
    printf("[Host:%s,Login:%s,Password:%s]\n", s1, s2, s3);
    free(s1);
    free(s2);
    free(s3);
    return 0;
}
