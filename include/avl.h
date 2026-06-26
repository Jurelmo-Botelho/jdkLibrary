#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    void *data;       
    int key;           
    int height;   
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

typedef struct {
    AVLNode *root;
    int total;      
} AVLTree;

AVLTree *avl_create(void);
void avl_destroy(AVLTree *tree, void (*free_data)(void *data));

//operações principais
int  avl_insert(AVLTree *tree, int key, void *data);
void *avl_search(AVLTree *tree, int key);
int  avl_remove(AVLTree *tree, int key);

void avl_inorder(AVLTree *tree, void (*visit)(void *data));


void avl_print(AVLTree *tree);

#endif