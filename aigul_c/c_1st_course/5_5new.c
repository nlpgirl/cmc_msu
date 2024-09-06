#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    int L;
    scanf("%d", &L);
    L++;
    char *s1 = (char*) malloc(L * sizeof(char));
    scanf("%s", s1);
    char *s2 = (char*) malloc(L * sizeof(char));
    scanf("%s", s2);
    char *s3 = (char*) malloc(L * sizeof(char));
    scanf("%s", s3);

    printf("%s%s%s", s3, s1, s2);
    free(s1);
    free(s2);
    free(s3);
}

