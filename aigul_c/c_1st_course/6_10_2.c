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

void insert_beginning(Node** root, int value){
		Node* new_node = malloc(sizeof(Node));
		new_node->x = value;
		new_node->next = *root;

		*root = new_node;
}

int main(void){
    int n, m, flag = 1;
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    fscanf(finput, "%d %d", &n, &m);
    int a[m][2], *adresses[m][3];
    for(int i=0; i<m; i++){
        for(int j=0; j<2; j++){
            fscanf(finput, "%d", &a[i][j]);
        }
    }
    Node *root = NULL, *prev_to_start = NULL, *start = NULL, *finish = NULL;
    for(int i=n; i>=1; i--){ //created a list from 1 to n
        insert_beginning(&root, i);
    }
    Node *previous = root;

    for(Node* cur=root; cur != NULL; cur=cur->next){
        for(int i=0; i<m; i++){
            if(cur->x == a[i][0]){
                start = cur;
                prev_to_start = previous;
            }
            if(cur->x == a[i][1]){
                finish = cur;
            }
            previous = cur;
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
