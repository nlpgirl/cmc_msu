#include <stdio.h>
#include <string.h>

int main(void){
    int n, k, l; char stroka[81]; char novstroka[81];
    scanf("%d", &n);
    scanf("%80s", stroka);
    l = strlen(stroka);
    k = n % l;
    for(int i=k; i<l; i++){
        int j = i-k;
        novstroka[j] = stroka[i];
    }
    for(int i=0; i<k; i++){
        int j = i+l-k;
        novstroka[j] = stroka[i];
    }
    novstroka[l] = '\0';
    printf("%s", novstroka);
    return 0;
}
