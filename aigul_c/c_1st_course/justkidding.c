typedef struct Node{
    int x;
    struct Node* next;
} Node;

void insert_end(Node** root, int value){
    Node* new_node = malloc(sizeof(Node));
    if(new_node == NULL){
        exit(1);
    }
    new_node->next = NULL;
    new_node->x = value;

    //рассмотрим случай, когда рут указывает на NULL (список пустой)
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
int main(void){
    //пусть для начала список состоит из одной структуры root
    Node* root = malloc(sizeof(Node));
    if(root == NULL){
        exit(2);
    }
    root->next = NULL;
    root->x = 15;
    //а теперь добавим в список новый элемент
    insert_end(&root, -2);

    return 0;
}

