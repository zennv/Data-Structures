#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct AVLNode_t{
    int data;
    struct AVLNode_t *left,*right;
    int height;
}AVLNode, *nodeptr;

typedef struct AVL_t{
    nodeptr root;
    unsigned int size;
}AVL, *avlptr;

nodeptr create_node(int value) {
    nodeptr newNode = (nodeptr) malloc(sizeof(AVLNode));
    newNode->data = value;
    newNode->height = 1;
    newNode->left = newNode->right = NULL;
    return newNode;
}

nodeptr search(nodeptr root, int value) {
    while (root != NULL) {
        if (value < root->data)
            root = root->left;
        else if (value > root->data)
            root = root->right;
        else
            return root;
    }
    return root;
}

int get_height(nodeptr node){
    if(node == NULL)
        return 0;
    return node->height;
}

int max(int a,int b){
    return (a > b)? a : b;
}

nodeptr right_rotate(nodeptr pivotNode) {

    nodeptr newParrent = pivotNode->left;
    pivotNode->left = newParrent->right;
    newParrent->right = pivotNode;

    pivotNode->height = max(get_height(pivotNode->left), get_height(pivotNode->right)) + 1;
    newParrent->height = max(get_height(newParrent->left), get_height(newParrent->right)) + 1;

    return newParrent;
}

nodeptr left_rotate(nodeptr pivotNode) {

    nodeptr newParrent = pivotNode->right;
    pivotNode->right = newParrent->left;
    newParrent->left = pivotNode;

    pivotNode->height = max(get_height(pivotNode->left), get_height(pivotNode->right)) + 1;
    newParrent->height = max(get_height(newParrent->left), get_height(newParrent->right)) + 1;

    return newParrent;
}

nodeptr leftCaseRotate(nodeptr node){
    return right_rotate(node);
}

nodeptr rightCaseRotate(nodeptr node){
    return left_rotate(node);
}

nodeptr leftRightCaseRotate(nodeptr node){
    node->left = left_rotate(node->left);
    return right_rotate(node);
}

nodeptr rightLeftCaseRotate(nodeptr node){
    node->right = right_rotate(node->right);
    return left_rotate(node);
}

int getBalanceFactor(nodeptr node){
    if(node == NULL)
        return 0;
    return get_height(node->left) - get_height(node->right);
}

nodeptr AVL_insert(avlptr avl, nodeptr node, int value){

    if(node == NULL) // udah mencapai leaf
        return create_node(value);
    if(value < node->data)
        node->left = AVL_insert(avl, node->left, value);
    else if(value > node->data)
    	node->right = AVL_insert(avl, node->right, value);

    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balanceFactor = getBalanceFactor(node);

    if(balanceFactor > 1 && value < node->left->data) // left skewed (left-left case)
        return leftCaseRotate(node);
    if(balanceFactor > 1 && value > node->left->data) // left zig-zag (left-right case)
		return leftRightCaseRotate(node);
    if(balanceFactor < -1 && value > node->right->data) // right skewed (kanan-kanan case)
        return rightCaseRotate(node);
    if(balanceFactor < -1 && value < node->right->data) // right zig-zag (right-left case)
        return rightLeftCaseRotate(node);

    return node;
}

nodeptr findMinNode(nodeptr node) {
    nodeptr currNode = node;
    while (currNode && currNode->left != NULL)
        currNode = currNode->left;
    return currNode;
}

nodeptr AVL_remove(nodeptr node, int value){
    if(node == NULL)
        return node;
    if(value > node->data)
    	node->right = AVL_remove(node->right, value);
    else if(value < node->data)
    	node->left = AVL_remove(node->left, value);
    else{
        nodeptr temp;
        if((node->left == NULL)||(node->right == NULL)){
            temp = NULL;
            if(node->left == NULL) temp = node->right;
            else if(node->right == NULL) temp = node->left;

            if(temp == NULL){
                temp = node;
                node = NULL;
            }
            else
                *node = *temp;  //move the node pointer node to point what temp points

            free(temp); /*since temp is no longer used. it's not because temp is pointing at 4 and then free 4.
                        the 4 is still exist just not pointed by anyone*/
        }
        else{
            temp = findMinNode(node->right);
            node->data = temp->data;
            node->right =AVL_remove(node->right, temp->data);
        }
    }

	if(node == NULL) return node;

    node->height = max(get_height(node->left),get_height(node->right)) + 1;

    int balanceFactor = getBalanceFactor(node);

    if(balanceFactor > 1 && getBalanceFactor(node->left) >= 0)
        return leftCaseRotate(node);

    if(balanceFactor > 1 && getBalanceFactor(node->left) < 0)
        return leftRightCaseRotate(node);

    if(balanceFactor < -1 && getBalanceFactor(node->right) <= 0)
        return rightCaseRotate(node);

    if(balanceFactor < -1 && getBalanceFactor(node->right) > 0)
        return rightLeftCaseRotate(node);

    return node;
}

avlptr init_avl() {
    avlptr avl = (avlptr) malloc(sizeof(AVL));
    avl->root = NULL;
    avl->size = 0u;
    return avl;
}

bool isEmpty_avl(avlptr avl) {
    return avl->root == NULL;
}

bool find_avl(avlptr avl, int value) {
    nodeptr temp = search(avl->root, value);
    if (temp == NULL)
        return false;

    if (temp->data == value)
        return true;
    else
        return false;
}

void insert_avl(avlptr avl, int value){
    if(!find_avl(avl, value)){
        avl->root = AVL_insert(avl, avl->root, value);
        avl->size++;
    }
}

void remove_avl(avlptr avl, int value){
    if(find_avl(avl, value)){
        avl->root = AVL_remove(avl->root, value);
        avl->size--;
    }
}

void inor(nodeptr root) {
    if (root){
        inor(root->left);
        printf("%d ", root->data);
        inor(root->right);
    }
}

void inorder(avlptr avl){
    inor(avl->root);
}

void pre(nodeptr root) {
    if (root){
        printf("%d ", root->data);
        pre(root->left);
        pre(root->right);
    }
}

void preorder(avlptr avl){
    pre(avl->root);
}

void post(nodeptr root) {
    if (root){
        post(root->left);
        post(root->right);
        printf("%d ", root->data);
    }
}

void postorder(avlptr avl){
    post(avl->root);
}

nodeptr cari_ayah(nodeptr root, int value){
    nodeptr ayah = NULL;
    while(root != NULL){
        if(value < root->data){
            ayah = root;
            root = root->left;
        }
        else if(value > root->data){
            ayah = root;
            root = root->right;
        }
        else return ayah;
    }
    return ayah;
}

void print_ayah(avlptr avl, int value){
    nodeptr temp = cari_ayah(avl->root, value);
    if(temp == NULL) printf("Yah ga ketemu D: %d\n", value);
    else printf("%d %d\n", temp->data, value);
}

int main(){
    avlptr yuta = init_avl();
    int n, x, q, y;
    scanf("%d", &n);
    while(n--){
        scanf("%d", &x);
        insert_avl(yuta, x);
    }
    scanf("%d", &q);
    while(q--){
        scanf("%d", &y);
        print_ayah(yuta, y);
    }
}
