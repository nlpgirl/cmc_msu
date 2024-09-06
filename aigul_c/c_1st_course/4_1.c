#include <stdio.h>
#include <string.h>
int answer(char *s1, char *s2, int len1, int len2){
    int flag = 1, count = 0, ans = 0, j;
    for(int konpref = len1 - 1; konpref >= 0; konpref--){ //konpref - конечный индекс префикса
        count = 0;
        flag = 1;
        for(int i = konpref; i >= 0; i--){
            if(flag){
                j = len2-konpref+i-1;
                if(i<0 || i > len1-1 || j<0 || j > len2-1) //проверка, выходит ли i или j за грани массива
                    break;
                    //flag = 0;
                if(s1[i] == s2[j]){
                    count += 1;
                }
                else flag = 0;
            }
        }

        if(flag){
            ans = count;
            break;
        }
    }
    return ans;
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
