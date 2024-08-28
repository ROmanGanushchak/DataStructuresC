#include <stdio.h>
#include <stdlib.h>

#define getHeight(node) ((node == NULL) ? 0 : (node->height))
#define getBalanceFactor(node) (getHeight(node->left) - getHeight(node->right))
#define max(a, b) (((a) > (b)) ? (a) : (b))

typedef struct userData {
    int id;
    char name[16];
    char surname[16];
    char date[12];
} UserData;

typedef struct node {
    UserData data;
    struct node *left;
    struct node *right;
    struct node *parent;
    unsigned int height;
} Node;

void strcopy(char str[], char source[]) {
    int i;
    for (i=0; source[i] != '\0'; i++) {
        str[i] = source[i];
    }
    str[i] = source[i];
}

void copyData(UserData *data, UserData *source) {
    data->id = source->id;
    strcopy(data->date, source->date);
    strcopy(data->name, source->name);
    strcopy(data->surname, source->surname);
}

void rotateLeft(Node *node, Node **root) {
    if (node == NULL || node->right == NULL) return;
    Node *newNode = node->right;

    node->right = newNode->left;
    if (newNode->left != NULL) newNode->left->parent = node;
    
    newNode->left = node;

    newNode->parent = node->parent;
    node->parent = newNode;

    if (newNode->parent != NULL) {
        if (newNode->parent->left == node) newNode->parent->left = newNode;
        else newNode->parent->right = newNode;
    } else *root = newNode;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
}

void rotateRight(Node *node, Node **root) {
    if (node == NULL || node->left == NULL) return;
    Node *newNode = node->left;

    node->left = newNode->right;
    if (newNode->right != NULL) newNode->right->parent = node;
    
    newNode->right = node;

    newNode->parent = node->parent;
    node->parent = newNode;

    if (newNode->parent != NULL) {
        if (newNode->parent->left == node) newNode->parent->left = newNode;
        else newNode->parent->right = newNode;
    } else *root = newNode;

    node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
}

void balanceAndUpdateHight(Node *node, Node **root) {
    int balanceFactor, subBalance, newHeight;
    for (; node != NULL; node = node->parent) {
        balanceFactor = getBalanceFactor(node);

        if (balanceFactor < -1) {
            subBalance = getBalanceFactor(node->right);
            if (subBalance > 0)  rotateRight(node->right, root);
            rotateLeft(node, root);
        } else if (balanceFactor > 1) {
            subBalance = getBalanceFactor(node->left);
            if (subBalance < 0) rotateLeft(node->left, root);
            rotateRight(node, root);
        }

        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

Node* searchElement(Node *node, int id) { 
    while (node != NULL && node->data.id != id) {
        if (node->data.id < id) node = node->right;
        else node = node->left;
    }

    return node;
}

void searchInterval(Node *node, int id1, int id2, char *isPrinted) {
    if (node == NULL) return;
    if (node->data.id < id1) searchInterval(node->right, id1, id2, isPrinted);
    else if (node->data.id > id2) searchInterval(node->left, id1, id2, isPrinted);
    else {
        searchInterval(node->left, id1, id2, isPrinted);
        if (*isPrinted == 1) printf("\n");
        else *isPrinted = 1;
        printf("%d %s %s %s", node->data.id, node->data.name, node->data.surname, node->data.date);
        searchInterval(node->right, id1, id2, isPrinted);
    }
}

void delete(Node *node, int id, Node **root) {
    while (node != NULL && node->data.id != id) {
        if (node->data.id < id) node = node->right;
        else node = node->left;
    }
    
    if (node == NULL) return;  

    if (node->right == NULL) {
        if (node->left != NULL) 
            node->left->parent = node->parent;
        
        if (node->parent != NULL) {
            if (node->parent->left == node) node->parent->left = node->left;
            else node->parent->right = node->left;
        } else  {
            (*root) = node->left;
            if (node->left != NULL)
                node->left->parent = NULL;
        }

        balanceAndUpdateHight(node->parent, root);

        free(node);
    } else {
        Node *replacement = node->right;
        while (replacement->left != NULL) replacement = replacement->left;

        if (replacement->parent->left == replacement)
            replacement->parent->left = replacement->right;
        else 
            replacement->parent->right = replacement->right;
        
        if (replacement->right != NULL) 
            replacement->right->parent = replacement->parent;

        copyData(&node->data, &replacement->data);
        balanceAndUpdateHight(node->parent, root);

        free(replacement);    
    }
}

void insert(Node **root) {
    Node* newNode = (Node*) malloc (sizeof(Node));
    scanf("%d %s %s %s", &newNode->data.id, newNode->data.name, newNode->data.surname, newNode->data.date);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 1;

    if (*root == NULL) {
        newNode->parent = NULL;
        *root = newNode;
        return;
    }

    Node *current = *root, *parent = NULL;
    while (current != NULL) {
        parent = current;
        if (current->data.id < newNode->data.id) current = current->right;
        else current = current->left;
    }

    newNode->parent = parent;
    if (parent->data.id < newNode->data.id) parent->right = newNode;
    else parent->left = newNode;
    
    balanceAndUpdateHight(parent, root);
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int id, id2, temp;
    char buffer[128];
    char type;
    char isPrinted = 0;
    Node *root = NULL, *node = NULL;

    while (scanf("%c ", &type) != EOF) {
        switch (type) {
            case 'i':;
                insert(&root);
            break;

            case 's':
                fgets(buffer, 128, stdin);
                id2 = 0;
                sscanf(buffer, "%d %d", &id, &id2);
                if (id2 == 0) {
                    node = searchElement(root, id);
                    if (node != NULL) {
                        if (isPrinted == 1) printf("\n");
                        else isPrinted = 1;
                        printf("%d %s %s %s", node->data.id, node->data.name, node->data.surname, node->data.date); 
                    }
                } else {
                    if (id2 < id) {
                        temp = id2;
                        id2 = id;
                        id = temp;
                    }
                    searchInterval(root, id, id2, &isPrinted);
                }
            break;

            case 'd':
                scanf("%d", &id);
                delete(root, id, &root);
            break;
        }
    }

    freeTree(root);
    return 0;
}