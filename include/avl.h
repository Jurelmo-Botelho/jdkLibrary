#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    void *data;   
    int   key;       
    int   height;   
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;


typedef struct {
    AVLNode *root;
    int      total;       
} AVLTree;

// Operações
AVLTree *avl_create(void);
int avl_insert(AVLTree *tree, int key, void *data);
void *avl_search(AVLTree *tree, int key);
int avl_remove(AVLTree *tree, int key);
void avl_inorder(AVLTree *tree, void (*visit)(void *data));
void avl_destroy(AVLTree *tree, void (*free_data)(void *data));

//Funções de Rotação
AVLNode* rotateLeft(AVLNode *r);
AVLNode* rotateRight(AVLNode *r);
AVLNode* rotateRightLeft(AVLNode *r);
AVLNode* rotateLeftRight(AVLNode *);

//Funções auxiliares 
AVLNode* newNode(int value);
int bigger(int a, int b);
int nodeHight(AVLNode* node);
int balancerFactor(AVLNode);

//Balancear
AVLNode* balancer(AVLNode *root);


#endif 
