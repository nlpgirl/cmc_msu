#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	int x;
	struct Node* next;
	struct Node* prev;
} Node;

void deallocate(Node* head){
	Node* curr = head;
	while (curr->next != NULL) {
		curr = curr->next;
		free(curr->prev);
	}
	free(curr);
}

Node *create_node(int value) {
    Node *new_node = malloc(sizeof(struct Node));
    new_node->x = value;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

int main(void){
    int n, m, flag = 1;
    FILE *finput = fopen("input.txt", "r");
    FILE *foutput = fopen("output.txt", "w");
    fscanf(finput, "%d %d", &n, &m);
    int a[m][2];
    for(int i=0; i<m; i++){
        for(int j=0; j<2; j++){
            fscanf(finput, "%d", &a[i][j]);
        }
    }
    Node* adress[n+1];
    adress[0] = NULL;
    adress[1] = create_node(1);
    Node* head = adress[1];

    for(int i=2; i<=n; i++){
        adress[i] = create_node(i);
        adress[i - 1]->next = adress[i]; //flip
        adress[i]->prev = adress[i - 1];
    }
    Node *start, *finish;
    for(int i=0; i<m; i++){
        flag = 1;
        start = adress[a[i][0]];
        finish = adress[a[i][1]];
        if(head->x == a[i][0]) flag = 0;
        if(flag){
            start->prev->next = finish->next;
            if(finish->next != NULL){
                finish->next->prev = start->prev;
            }
            finish->next = head;
            head->prev = finish;
            start->prev = NULL;
            head = start;
        }
    }
    for(Node* curr = head; curr != NULL; curr = curr->next) {
        fprintf(foutput, "%d ", curr->x);
    }
    fclose(finput);
    fclose(foutput);
    deallocate(head);
    return 0;
}

