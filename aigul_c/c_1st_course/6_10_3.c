
#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int x;
    struct Node* next;
} Node;

void deallocate(Node** root){

		Node* curr = *root;
		while(curr != NULL){
				Node* aux = curr;
				curr = curr->next;
				free(aux);
		}
		*root = NULL;
}

Node* insert_beginning(Node** root, int value){
		Node* new_node = malloc(sizeof(Node));
		new_node->x = value;
		new_node->next = *root;

		*root = new_node;
		return new_node;
}

int main(void){
    int n, m, start, finish, flag = 1;
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    fscanf(finput, "%d %d", &n, &m);
    int a[m][2];
    for(int i=0; i<m; i++){
        for(int j=0; j<2; j++){
            fscanf(finput, "%d", &a[i][j]);
        }
    }
    Node *root = NULL, *prev_to_start = NULL, *pstart = NULL, *pfinish = NULL, *adress[n+2], *adress_of_previous[n+1];
    adress[n+1] = NULL;
    adress[0] = NULL;

    for(int i=n; i>=1; i--){ //created a list from 1 to n
        adress[i] = insert_beginning(&root, i);
        adress_of_previous[i] = adress[i-1];
    }
    //prev_to_start = adress_of_next[a[0][0]-1];
    for(int i=0; i<m; i++){
        start = a[i][0]; finish = a[i][1];
        prev_to_start = adress_of_previous[start];
        flag = 1;
        if(root->x == start) flag = 0;
        if(flag){
            pstart = adress[start];
            pfinish = adress[finish];

            adress_of_previous[root->x] = pfinish;
            adress_of_previous[start] = NULL;
            adress_of_previous[pfinish->next->x] = prev_to_start;
            //
            prev_to_start->next = pfinish->next;
            pfinish->next = root;
            root = pstart;

            printf("%d\n", i);
            return 0;
        }


    }



    for(Node* cur=root; cur != NULL; cur=cur->next){
        fprintf(foutput, "%d ", cur->x);
    }

    deallocate(&root);
    fclose(finput);
    fclose(foutput);
    return 0;
}
