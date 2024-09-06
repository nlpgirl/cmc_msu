#include <stdio.h>
#include <string.h>

int recur(int n, char tkgorod, char masgorodov[][2], int *permission, int count){
    int tcount, maxim=0; //flag = 0;
    char perv, posl;
    for(int i=0; i<n; i++){
        if (permission[i]){
            perv = masgorodov[i][0];
            if (perv == tkgorod){
                permission[i] = 0;
                //count++;
                //flag = 1;
                tcount = recur(n, masgorodov[i][1], masgorodov, permission, count);
                if(tcount > maxim) maxim = tcount;
            }
        }
    }
    return 1+maxim;


}

int main(void){
    int n;
    scanf("%d", &n);
    char masgorodov[n][2]; //хранится начальная и конечная буква
    char goroda[n][21];
    int permission[n];
    for(int i=0; i<n; i++){
        permission[i] = 1;
    }
    int count = 0;
    /*
    for(int i=0; i<2; i++){ //заполним массив начальных и конечных букв ашками
        for(int j=0; j<10; j++){
            masgorodov[j][i] = 'a';
        }
    }
    */
    for(int i = 0; i < n; i++){ //заполняем двумерный массив названиями городов
        scanf("%20s", goroda[i]);
        int lentstr = strlen(goroda[i]);
        masgorodov[i][0] = goroda[i][0]; //формируем массив из первой и последней буквы слова
        masgorodov[i][1] = goroda[i][lentstr-1];
    }


    for(int i = 0; i < n; i++){
        printf("%s %d\n",goroda[i], recur(n, masgorodov[i][1], masgorodov, permission, count));
    }


    return 0;
}
