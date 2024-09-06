#include <stdio.h>
#include <string.h>
#include <locale.h>

int recur(int n, int indposl, char poslbukva, char pervposl[][2], int permission){
    int maxim = 0;
    int maximsled;

    permission -= (1<<(n-indposl-1));
    for (int i=0; i<n; i++){

        if((permission&(1<<(n-i-1))) && (pervposl[i][0] == poslbukva)){
            poslbukva = pervposl[i][1];
            maximsled = recur(n, i, poslbukva, pervposl, permission);
            if (maximsled >= maxim){
                maxim = maximsled;
            }
        }
    }
    //int massiv[2] = {maxim+1, indposl};
    printf("%d\n", indposl);
    return maxim;


}
int poisk(int count, char *searchedcity, char previouscities[][21]){
    for (int i=0; i<count; i++){
        if(strcmp(searchedcity, previouscities[i]) == 0) return 1;
    }
    return 0;
}

int main(void){
    setlocale(LC_ALL, "Rus");
    int n;
    scanf("%d", &n);
    char pervposl[n][2]; //хранится начальная и конечная буква
    char goroda[n][21];
    int permission = (1<<n)-1;
    /*
    for(int i=0; i<n; i++){
        permission[i] = 1;
    }
    */
    int dlini[n];
    for(int i = 0; i < n; i++){ //заполняем двумерный массив названиями городов
        scanf("%20s", goroda[i]);
        int lentstr = strlen(goroda[i]);
        dlini[i] = lentstr;
        pervposl[i][0] = goroda[i][0]; //формируем массив из первой и последней буквы слова
        pervposl[i][1] = goroda[i][lentstr-1];
    }
    //int mas = recur(n, 2, 'N', pervposl, permission);
    printf("%s %d\n", goroda[2], recur(n, 2, 'N', pervposl, permission));
    /*
    char poslbukva;
    int ans[n];
    int maxim = 0;
    for(int i = 0; i < n; i++){
        poslbukva = pervposl[i][1];
        ans[i] = recur(n, i, poslbukva, pervposl, permission);
        if(ans[i] > maxim) maxim = ans[i];
    }
    printf("%d\n", maxim);

    char res[n][21];
    int count = 0;
    for(int i = 0; i < n; i++){
        if(ans[i] == maxim){
            if (poisk(count, goroda[i], res) == 0){
                strncpy(res[count], goroda[i], dlini[i]+5);
                count += 1;
            }
        }
    }
    for(int i=0; i<count; i++){
        puts(res[i]);
    }
    */
    return 0;
}
