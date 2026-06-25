#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

static int max(int a, int b) {
    return (a > b) ? a : b;
}

static int height(AVLNode *node) {
    return (node == NULL) ? -1 : node->height;
}

static int balance_factor(AVLNode *node) {
    if (node == NULL) return 0;
    return height(node->left) - height(node->right);
}

static AVLNode *create_node(int key, void *data) {
    AVLNode *node = (AVLNode*)malloc(sizeof(AVLNode));
    if (node == NULL) {
        printf("Erro: Falha na alocação\n");
        return NULL;
    }
    node->key = key;
    node->data = data;
    node->height = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

//Rotações

static AVLNode *rotate_left(AVLNode *r) {
    AVLNode *y = r->right;
    AVLNode *b = y->left;
    
    y->left = r;
    r->right = b;
    
    r->height = max(height(r->left), height(r->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    
    return y;
}

static AVLNode *rotate_right(AVLNode *r) {
    AVLNode *y = r->left;
    AVLNode *b = y->right;
    
    y->right = r;
    r->left = b;
    
    r->height = max(height(r->left), height(r->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    
    return y;
}

static AVLNode *rotate_left_right(AVLNode *r) {
    r->left = rotate_left(r->left);
    return rotate_right(r);
}

static AVLNode *rotate_right_left(AVLNode *r) {
    r->right = rotate_right(r->right);
    return rotate_left(r);
}

//Balancear
static AVLNode *balance(AVLNode *node) {
    if (node == NULL) return NULL;
    
    int fb = balance_factor(node);
    
    if (fb > 1) {
        if (balance_factor(node->left) >= 0)
            return rotate_right(node);          
        else
            return rotate_left_right(node);     
    }
    
    if (fb < -1) {
        if (balance_factor(node->right) <= 0)
            return rotate_left(node);           
        else
            return rotate_right_left(node);    
    }
    
    return node;
}

//Inserção
static AVLNode *insert_recursive(AVLNode *node, int key, void *data) {
    if (node == NULL)
        return create_node(key, data);
    
    if (key < node->key)
        node->left = insert_recursive(node->left, key, data);
    else if (key > node->key)
        node->right = insert_recursive(node->right, key, data);
    else {
        printf("Erro: Chave %d já existe\n", key);
        return node;
    }
    
    node->height = max(height(node->left), height(node->right)) + 1;
    return balance(node);
}

//Busca 
static void *search_recursive(AVLNode *node, int key) {
    if (node == NULL) return NULL;
    
    if (key == node->key)
        return node->data;
    else if (key < node->key)
        return search_recursive(node->left, key);
    else
        return search_recursive(node->right, key);
}

//Remoção
static AVLNode *find_min(AVLNode *node) {
    if (node == NULL) return NULL;
    if (node->left == NULL) return node;
    return find_min(node->left);
}

static AVLNode *remove_recursive(AVLNode *node, int key) {
    if (node == NULL) return NULL;
    
    if (key < node->key) {
        node->left = remove_recursive(node->left, key);
    }
    else if (key > node->key) {
        node->right = remove_recursive(node->right, key);
    }
    else {
        if (node->left == NULL || node->right == NULL) {
            AVLNode *temp = node->left ? node->left : node->right;
            
            if (temp == NULL) {
                free(node);
                return NULL;
            } else {
                *node = *temp;
                free(temp);
            }
        }
        else {
            AVLNode *sucessor = find_min(node->right);
            node->key = sucessor->key;
            node->data = sucessor->data;
            node->right = remove_recursive(node->right, sucessor->key);
        }
    }
    
    if (node == NULL) return NULL;
    
    node->height = max(height(node->left), height(node->right)) + 1;
    return balance(node);
}

//Funções públicas

AVLTree *avl_create(void) {
    AVLTree *tree = (AVLTree*)malloc(sizeof(AVLTree));
    if (tree == NULL) {
        printf("Erro: Falha ao criar árvore\n");
        return NULL;
    }
    tree->root = NULL;
    tree->total = 0;
    return tree;
}

void avl_destroy(AVLTree *tree, void (*free_data)(void *)) {
    if (tree == NULL) return;
    destroy_recursive(tree->root, free_data);
    tree->root = NULL;
    tree->total = 0;
    free(tree);
}

int avl_insert(AVLTree *tree, int key, void *data) {
    if (tree == NULL || data == NULL) return 0;
    
    if (avl_search(tree, key) != NULL) {
        printf("Erro: Chave %d já existe\n", key);
        return 0;
    }
    
    tree->root = insert_recursive(tree->root, key, data);
    tree->total++;
    return 1;
}

void *avl_search(AVLTree *tree, int key) {
    if (tree == NULL) return NULL;
    return search_recursive(tree->root, key);
}

int avl_remove(AVLTree *tree, int key) {
    if (tree == NULL || tree->root == NULL) return 0;
    
    if (avl_search(tree, key) == NULL) {
        printf("Erro: Chave %d não encontrada\n", key);
        return 0;
    }
    
    tree->root = remove_recursive(tree->root, key);
    tree->total--;
    return 1;
}

void avl_inorder(AVLTree *tree, void (*visit)(void *)) {
    if (tree == NULL || visit == NULL) return;
    inorder_recursive(tree->root, visit);
}

//IMPRIMIR

static void print_recursive(AVLNode *node, int level) {
    if (node == NULL) return;
    
    print_recursive(node->right, level + 1);
    
    for (int i = 0; i < level; i++)
        printf("    ");
    printf("Key: %d (h=%d)\n", node->key, node->height);
    
    print_recursive(node->left, level + 1);
}

void avl_print(AVLTree *tree) {
    if (tree == NULL) {
        printf("Árvore vazia\n");
        return;
    }
    printf("\n=== ÁRVORE AVL ===\n");
    print_recursive(tree->root, 0);
    printf("Total: %d nós\n", tree->total);
}