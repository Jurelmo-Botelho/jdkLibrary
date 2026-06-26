#ifndef FILES_H
#define FILES_H

#include "avl.h"
#include "book.h"
#include "user.h"
#include "loan.h"

// Carregar dados
int files_load_books(AVLTree *book_tree, const char *filename);
int files_load_users(AVLTree *user_tree, const char *filename);

// Guardar dados
int files_save_books(AVLTree *book_tree, const char *filename);
int files_save_users(AVLTree *user_tree, const char *filename);

// Função auxiliar
int files_count_lines(const char *filename);

#endif