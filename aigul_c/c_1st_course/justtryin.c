#include <stdio.h>
#include <string.h>

#define MAX_LEN 11

int main() {
    int n;
    char string[MAX_LEN];
    int count = 0;
    int i, j;

    scanf("%d", &n);

    char strs[n][MAX_LEN];

    for (i = 0; i < n; i++) {
        scanf("%s", string);
        int match = 0;
        for (j = 0; j < count; j++) {
            if (strcmp(string, strs[j]) == 0) {
                match = 1;
                break;
            }
        }
        if (!match) {
            strcpy(strs[count], string);
            count++;
        }
    }
    printf("%d\n", count);
    return 0;
}
