#include <stdio.h>
#include <string.h>

int recur(int n, int index, char *first, char *last, int *used){
    used[index] = 1;
    int max = 0, newmax;
    for(int i=0; i<n; i++){
        if((used[i] == 0) && (first[i] == last[index])){
            newmax = recur(n, i, first, last, used);
            if(newmax > max){
                max = newmax;
            }
        }
    }
    used[index] = 0;
    return max+1;
}

int search(int count, char res[][21], char *city, int length){
    for(int i=0; i<count; i++){
        if(strncmp(city, res[i], length+1) == 0){
            return 0;
        }
    }
    return 1;
}
int main(void){
    int n;
    scanf("%d", &n);
    char cities[n][21];
    char first[n], last[n];
    int lengths[n];
    for(int i=0; i<n; i++){
        scanf("%21s", cities[i]);
        first[i] = cities[i][0];
        lengths[i] = strlen(cities[i]);
        last[i] = cities[i][lengths[i]-1];
    }
    int ans[n], used[n];
    for(int i=0; i<n; i++){
        used[i] = 0;
    }
    int max = 0;
    for(int i=0; i<n; i++){
        ans[i] = recur(n, i, first, last, used);
        if(ans[i] > max) max = ans[i];
    }
    printf("%d\n", max);
    char res[n][21];
    int count = 0;
    for(int i=0; i<n; i++){
        if((ans[i] == max) && search(count, res, cities[i], lengths[i])){
            strncpy(res[count], cities[i], lengths[i]+1);
            count++;
        }
    }
    for(int i=0; i<count; i++){
        puts(res[i]);
    }

    return 0;
}
