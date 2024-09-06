#include <stdio.h>
#include <string.h>
#include <locale.h>

int recur(int n, int indposl, char poslbukva, char pervposl[][2], int *permission){
    int maxim = 0;
    int maximsled;
    int copypermission[n];
    for (int i=0; i<n; i++){
        copypermission[i] = permission[i];
    }
    copypermission[indposl] = 0;
    for (int i=0; i<n; i++){
        if((copypermission[i]) && (pervposl[i][0] == poslbukva)){
            //copypermission[i] = 0;
            poslbukva = pervposl[i][1];
            maximsled = recur(n, i, poslbukva, pervposl, copypermission);
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
    setlocale(LC_ALL, "Rus");
    int n;
    scanf("%d", &n);
    char pervposl[n][2];
    char goroda[n][21];
    int permission[n];
    for(int i=0; i<n; i++){
        permission[i] = 1;
    }
    int dlini[n];
    for(int i = 0; i < n; i++){
        scanf("%20s", goroda[i]);
        int lentstr = strlen(goroda[i]);
        dlini[i] = lentstr;
        pervposl[i][0] = goroda[i][0];
        pervposl[i][1] = goroda[i][lentstr-1];
    }
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
    return 0;
}
