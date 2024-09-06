#include <stdio.h>
#include <stdlib.h>

typedef struct binaryTree_s {
    int data;
    struct binaryTree_s *left;
    struct binaryTree_s *right;
} binaryTree;

binaryTree *createNode(int data){
    binaryTree *newNode = (binaryTree *)malloc(sizeof(binaryTree));
    if (newNode == NULL){
        printf("Malloc returned NULL\n");
        exit(1);
    }
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void push(binaryTree **tree, int data){

    binaryTree *tmp = *tree;
    if(tmp == NULL){
        *tree = createNode(data);
    }
    else if(data < tmp->data){
        if(tmp->left == NULL){
            tmp->left = createNode(data);
        }
        else{
            push(&(tmp->left), data);
        }
    }
    else if(data > tmp->data){
        if(tmp->right == NULL){
            tmp->right = createNode(data);
        }
        else{
            push(&(tmp->right), data);
        }
    }
}
void printInDepthRecursively(binaryTree *tree){
    if(tree == NULL){
        return;
    }
    else{
        printf("%d ", tree->data);
        if(tree->left != NULL){
            printInDepthRecursively(tree->left);
        }
        if(tree->right != NULL){
            printInDepthRecursively(tree->right);
        }
    }
}

int main(){
    binaryTree *tree = NULL;
    push(&tree, 5);
    push(&tree, 7);
    push(&tree, 3);
}
