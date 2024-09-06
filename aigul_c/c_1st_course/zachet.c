#include <stdio.h>
#include <string.h>

void recombine(char* s) {
	int n = strlen(s);
	char aux;
	if(n%2 != 2){
        for(int i=0; i<(n/4); i++){
            aux = s[(n/2) + i];
            s[(n/2) + i] = s[i];
            s[i] = aux;
        }
	}
	recombine(s);
}

