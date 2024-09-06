#include <stdio.h>
#include <string.h>

int answer(char *s1, char *s2, int len1, int len2){

    char s[len1+len2+10];

    strncpy(s, s1, len1+10);
    s[len1] = ' ';
    strncat(s, s2, len2+10);
    int newlen = len1+len2+1, k;
    int p[newlen];

    for(int i=0; i <newlen; i++){
        p[i] = 0;
    }
    for (int i=1; i<newlen; i++){
        k = p[i-1];
        while(k>0 && s[k] != s[i]){
            k = p[k-1];
        }
        if (s[k] == s[i]){
            k += 1;
        }
        p[i] = k;
    }
    return p[newlen-1];
}

int main(void){
    char s1[1000005], s2[1000005];
    scanf("%1000003s", s1);
    scanf("%1000003s", s2);

    int len1, len2;
    len1 = strlen(s1);
    len2 = strlen(s2);


    printf("%d ", answer(s1, s2, len1, len2));
    printf("%d", answer(s2, s1, len2, len1));

    return 0;
}
