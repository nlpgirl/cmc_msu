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

void insert_after(Node* node, int value){
		Node* new_node = malloc(sizeof(Node));
		new_node->x = value;
		new_node->next = node->next;
		node->next = new_node;
}

void insert_sorted(Node** root, int value){
		if (*root == NULL || (**root).x >= value){
				insert_beginning(root, value);
				return;
		}
		Node* curr = *root;
		while (curr->next != NULL) {
				if (curr->next->x >= value) {
						break;
				}
				curr = curr->next;
		}
		insert_after(curr, value);
}

int main(void){
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    int y;
    Node* root = NULL;

    while(fscanf(finput, "%d", &y) == 1){
        insert_sorted(&root, y);
    }
    for(Node* cur=root; cur != NULL; cur=cur->next){
        fprintf(foutput, "%d ", cur->x);
        //printf("%d ", cur->x);
    }
    fclose(finput);
    fclose(foutput);
    deallocate(&root);

    return 0;
}
