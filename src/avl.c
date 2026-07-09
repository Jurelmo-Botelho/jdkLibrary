#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

int avl_height(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}

int avl_max(int a, int b)
{
    return (a > b) ? a : b;
}

int avl_balance_factor(AVLNode *node)
{
    if (node == NULL)
        return 0;
    return avl_height(node->left) - avl_height(node->right);
}


AVLNode *avl_create_node(int key, void *data)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));

    if (node == NULL)
    {
        printf("Erro: falha na alocação de memória.\n");
        return NULL;
    }

    node->key = key;
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}


AVLNode *avl_rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + avl_max(avl_height(y->left), avl_height(y->right));
    x->height = 1 + avl_max(avl_height(x->left), avl_height(x->right));

    return x;
}

AVLNode *avl_rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = 1 + avl_max(avl_height(x->left), avl_height(x->right));
    y->height = 1 + avl_max(avl_height(y->left), avl_height(y->right));

    return y;
}


AVLNode *avl_insert(AVLNode *node, int key, void *data)
{
    if (node == NULL)
        return avl_create_node(key, data);

    if (key < node->key)
        node->left = avl_insert(node->left, key, data);
    else if (key > node->key)
        node->right = avl_insert(node->right, key, data);
    else
    {
        printf("Erro: chave duplicada (%d).\n", key);
        return node;
    }

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));

    int balance = avl_balance_factor(node);

    /* LL */
    if (balance > 1 && key < node->left->key)
        return avl_rotate_right(node);

    /* RR */
    if (balance < -1 && key > node->right->key)
        return avl_rotate_left(node);

    /* LR */
    if (balance > 1 && key > node->left->key)
    {
        node->left = avl_rotate_left(node->left);
        return avl_rotate_right(node);
    }

    /* RL */
    if (balance < -1 && key < node->right->key)
    {
        node->right = avl_rotate_right(node->right);
        return avl_rotate_left(node);
    }

    return node;
}


AVLNode *avl_min_node(AVLNode *node)
{
    AVLNode *current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}


AVLNode *avl_search(AVLNode *root, int key)
{
    if (root == NULL)
        return NULL;

    if (key == root->key)
        return root;

    if (key < root->key)
        return avl_search(root->left, key);

    return avl_search(root->right, key);
}


AVLNode *avl_remove(AVLNode *root, int key)
{
    if (root == NULL)
        return NULL;

    if (key < root->key)
        root->left = avl_remove(root->left, key);

    else if (key > root->key)
        root->right = avl_remove(root->right, key);

    else
    {
        /* 0 ou 1 filho */
        if (root->left == NULL || root->right == NULL)
        {
            AVLNode *temp = root->left ? root->left : root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
            {
                *root = *temp;
            }

            free(temp);
        }
        else
        {
            AVLNode *temp = avl_min_node(root->right);

            root->key = temp->key;
            root->data = temp->data;

            root->right = avl_remove(root->right, temp->key);
        }
    }

    if (root == NULL)
        return NULL;

    root->height = 1 + avl_max(avl_height(root->left), avl_height(root->right));

    int balance = avl_balance_factor(root);

    /* LL */
    if (balance > 1 && avl_balance_factor(root->left) >= 0)
        return avl_rotate_right(root);

    /* LR */
    if (balance > 1 && avl_balance_factor(root->left) < 0)
    {
        root->left = avl_rotate_left(root->left);
        return avl_rotate_right(root);
    }

    /* RR */
    if (balance < -1 && avl_balance_factor(root->right) <= 0)
        return avl_rotate_left(root);

    /* RL */
    if (balance < -1 && avl_balance_factor(root->right) > 0)
    {
        root->right = avl_rotate_right(root->right);
        return avl_rotate_left(root);
    }

    return root;
}

//Libertação
void avl_destroy(AVLNode *root)
{
    if (root == NULL)
        return;

    avl_destroy(root->left);
    avl_destroy(root->right);

    free(root);
}