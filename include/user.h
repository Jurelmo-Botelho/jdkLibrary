#ifndef USER_H
#define USER_H

#include "types.h"
#include "avl.h"

typedef struct {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    Role role;
    char password_hash[MAX_HASH_STRING];
    int active_loans;
} User;

//ID dinâmico
int user_get_next_id(void);
void user_load_next_id(void);
void user_save_next_id(void);
void user_update_next_id(int max_id);

// Criação com senha (calcula hash)
User* user_create(int id, const char *name, const char *phone,
                  Role role, const char *password);

// Criação com hash já calculado (para carregar do ficheiro)
User* user_create_with_hash(int id, const char *name, const char *phone,
                            Role role, const char *password_hash);

// Destruição
void user_destroy(User *user);
void user_destroy_callback(void *data);

// CRUD
int user_insert(AVLTree *tree, User *user);
User* user_find(AVLTree *tree, int id);
int user_update(AVLTree *tree, int id, const char *name, const char *phone);
int user_delete(AVLTree *tree, int id);

// Impressão
void user_print(const User *user);
void user_print_callback(void *data);
void user_list_all(AVLTree *tree);

// Verificação
int user_can_borrow(const User *user);

// Cria admin default (para quando o ficheiro está vazio)
User* user_create_default_admin();

#endif