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

void insert_value_beginning(Node** root, int value){
		Node* new_node = malloc(sizeof(Node));
		new_node->x = value;
		new_node->next = *root;

		*root = new_node;
}

void insert_beginning(Node** root, Node* previous, Node* node){ //приписывает в начало узел, который дали
		previous->next = node->next;
		node->next = *root;
		*root = node;
}

void insert_after(Node* tail, Node* detachfrom, Node* node){ //приписывает узел node после tail
    detachfrom->next = node->next;
    node->next = tail->next;
    tail->next = node;
}

int main(void){
    int n, m;
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    fscanf(finput, "%d %d", &n, &m);
    int **a = (int**)malloc(sizeof(int*)*m);
    for(int i=0; i<m; i++){
        a[i] = (int*)malloc(sizeof(int)*2);
    }
    for(int i=0; i<m; i++){
        for(int j=0; j<2; j++){
            fscanf(finput, "%d", &a[i][j]);
        }
    }
    Node *root = NULL, *last_moved = NULL;
    int start = 0;
    for(int i=n; i>=1; i--){ //created a list from 1 to n
        insert_value_beginning(&root, i);
    }
    Node *previous = root;
    for(int i=0; i<m; i++){
        start = 0;
        for(Node* cur=root; cur != NULL; cur=previous->next){
            if(cur == root && cur->x == a[i][0]) break;
            if(start){
                insert_after(last_moved, previous, cur);
                last_moved = cur;
            }
            if(cur->x == a[i][0]){
                start = 1;
                insert_beginning(&root, previous, cur);
                last_moved = cur;
            }
            if(cur->x == a[i][1]) break;
            if(start == 0) previous = cur;
        }
    }
    for(Node* cur=root; cur != NULL; cur=cur->next){
        fprintf(foutput, "%d ", cur->x);
    }
    for (int i = 0; i < m; i++) {
        free(a[i]);
    }
    free(a);
    deallocate(&root);
    fclose(finput);
    fclose(foutput);
    return 0;
}
