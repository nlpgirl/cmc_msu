#include <stdio.h>
#include <string.h>

int recur(int n, int index, char *first, char *last, int *permission){
    int maximsled, maxim = 0;

    permission[index] = 1;
    for (int i=0; i<n; i++){
        if((permission[i] == 0) && (first[i] == last[index])){
            maximsled = recur(n, i, first, last, permission);
            if (maximsled > maxim){
                maxim = maximsled;
            }
        }
    }
    return maxim+1;


}
int poisk(int count, char *searchedcity, char previouscities[][21]){
    for (int i=0; i<count; i++){
        if(strcmp(searchedcity, previouscities[i]) == 0) return 1;
    }
    return 0;
}

int main(void){
    int n;
    scanf("%d", &n);
    char first[n], last[n]; //хранится начальная и конечная буква
    char goroda[n][21];

    int permission[n];
    for (int i=0; i<n; i++){
        permission[i] = 0;
    }
    //int permission = (1<<n)-1;


    int dlini[n];
    for(int i = 0; i < n; i++){ //заполняем двумерный массив названиями городов
        scanf("%21s", goroda[i]);
        int lentstr = strlen(goroda[i]);
        dlini[i] = lentstr;
        first[i] = goroda[i][0]; //формируем массив из первой и последней буквы слова
        last[i] = goroda[i][lentstr-1];
    }

    int ans[n];
    int maxim = 0;
    for(int i = 0; i < n; i++){
        ans[i] = recur(n, i, first, last, permission);
        if(ans[i] > maxim) maxim = ans[i];
    }
    printf("%d\n", maxim);

    char res[n][21];
    int count = 0;
    for(int i = 0; i < n; i++){
        if(ans[i] == maxim){
            if (poisk(count, goroda[i], res) == 0){
                strncpy(res[count], goroda[i], dlini[i]+1);
                count += 1;
            }
        }
    }
    for(int i=0; i<count; i++){
        puts(res[i]);
    }
    return 0;
}
