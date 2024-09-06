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

void insert_end(Node** root, int value){
    Node* new_node = malloc(sizeof(Node));
    new_node->next = NULL;
    new_node->x = value;

    if(*root == NULL){
        *root = new_node;
        return;
    }

    Node* curr = *root;
    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = new_node;
}

void remove_element(Node** root, int value){
    if(*root == NULL){
        return;
    }
    if((**root).x == value){
        Node* to_remove = *root;
        *root = (*root)->next;
        free(to_remove);
        return;
    }
    for(Node* curr = *root; curr->next != NULL; curr = curr->next){
        if(curr->next->x == value){
            Node* to_remove = curr->next;
            curr->next = curr->next->next;
            free(to_remove);
            return;
        }
    }
}

int main(void){
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    Node *root = NULL;
    int y, flag = 1;
    fscanf(finput,"%d", &y);
    while(y != -1){
        insert_end(&root, y);
        fscanf(finput,"%d", &y);
    }
    fscanf(finput,"%d", &y);
    while(y != -1){
        flag = 1;

        for(Node* cur=root; cur != NULL; cur=cur->next){
            if(cur->x == y){
                flag = 0;
                break;
            }
        }
        if(flag) fprintf(foutput, "%d ", y);
        fscanf(finput,"%d", &y);
    }
    fclose(finput);
    fclose(foutput);
    deallocate(&root);
    return 0;
}
