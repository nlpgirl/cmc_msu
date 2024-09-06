#include <stdio.h>
#include <stdlib.h>

struct list {
    int key;
    struct list *next;
    struct list *sublist_head;
};

int examine(struct list *garland){
    if (garland == NULL) return -1;
    struct list *curstart = NULL, *cur = NULL;
    int curlen = 0, curbrightness = 0, maxlength = 0, maxbrightness = 0;
    for(curstart = garland; curstart != NULL; curstart=curstart->sublist_head){
        curlen = 0;
        curbrightness = 0;
        for(cur = curstart; cur != NULL; cur=cur->next){
            curlen++;
            curbrightness += cur->key;
        }
        if(curlen >= maxlength){
            if(curlen == maxlength){
                if(curbrightness > maxbrightness){
                    maxbrightness = curbrightness;
                }
            }
            else{
                maxbrightness = curbrightness;
            }
            maxlength = curlen;
        }
    }
    return maxbrightness;
}
int main(){
    struct list garland;
    garland = {}
}
