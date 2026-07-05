#ifndef AVL_H
#define AVL_H

#include "types.h"

typedef struct AVLNode
{
    int key;
    void *data;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;

} AVLNode;

AVLNode *avl_create_node(int key, void *data);
AVLNode *avl_insert(AVLNode *root, int key, void *data);
AVLNode *avl_remove(AVLNode *root, int key);
AVLNode *avl_search(AVLNode *root, int key);

/* Utilitários */
int avl_height(AVLNode *node);
int avl_balance_factor(AVLNode *node);
int avl_max(int a, int b);
AVLNode *avl_min_node(AVLNode *root);

/* Rotações */
AVLNode *avl_rotate_left(AVLNode *node);
AVLNode *avl_rotate_right(AVLNode *node);

/* Libertação */
void avl_destroy(AVLNode *root);

#endif