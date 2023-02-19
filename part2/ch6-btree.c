/*
ADT binary tree
inorder, preorder, postorder
*/

#include <stdio.h>
#include <stdlib.h>

typedef char DATA;

struct node {
    DATA d;
    struct node *left, *right;
};

typedef struct node NODE;
typedef NODE *BTREE;

void inorder(BTREE root){
    if(root != NULL){
        inorder(root -> left);
        printf("%c  ", root  -> d);
        inorder(root -> right);
    }
}

void preorder(BTREE root){
    if(root != NULL){
        printf("%c  ", root  -> d);
        preorder(root -> left);
        preorder(root -> right);
    }
}

void postorder(BTREE root){
    if(root != NULL){
        postorder(root -> left);
        postorder(root -> right);
        printf("%c  ", root  -> d);
    }
}

BTREE new_node(void){
    return malloc(sizeof(NODE));
}

BTREE init_node(DATA d1, BTREE p1, BTREE p2){
    BTREE t = new_node();
    t -> d = d1;
    t -> left = p1;
    t -> right = p2;
}

BTREE create_tree(DATA a[], int i, int size){
    if(i >= size){
        return NULL;
    }
    else {
        return ( init_node(a[i], create_tree(a, 2*i+1, size), create_tree(a, 2*i+2, size) ));
    }
}

int main(void){

    char d[] = {'a', 'b', 'c', 'd', 'e'};

    BTREE b;
    b = create_tree(d, 0, 5);

    printf("inorder: ");
    inorder(b);
    printf("\n\n");

    printf("preorder: ");
    preorder(b);
    printf("\n\n");

    printf("postorder: ");
    postorder(b);
    printf("\n\n");

    return 0;

}