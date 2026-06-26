#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"
#include "auth.h"
#include "avl.h"

static int next_user_id = 2; 

void user_load_next_id(void)
{
    FILE *file = fopen("data/next_id.txt", "r");
    if (file == NULL) {
        next_user_id = 2; 
        return;
    }
    
    fscanf(file, "%d", &next_user_id);
    fclose(file);
}


void user_save_next_id(void)
{
    FILE *file = fopen("data/next_id.txt", "w");
    if (file == NULL) return;
    
    fprintf(file, "%d", next_user_id);
    fclose(file);
}

void user_update_next_id(int max_id)
{
    if (max_id >= next_user_id) {
        next_user_id = max_id + 1;
    }
}

int user_get_next_id(void) {
    return next_user_id++;
}

User* user_create(int id, const char *name, const char *phone,
                  Role role, const char *password)
{
    if (password == NULL) return NULL;

    if (id == 0) {
        id = user_get_next_id();
    }

    if (id <= 0) return NULL;

    unsigned long hash = auth_hash_password(password);
    char hash_string[MAX_HASH_STRING];
    auth_hash_to_string(hash, hash_string);

    return user_create_with_hash(id, name, phone, role, hash_string);
}

// Cria utilizador com hash já calculado
User* user_create_with_hash(int id, const char *name, const char *phone,
                            Role role, const char *password_hash)
{
    if (id <= 0 || name == NULL || phone == NULL || password_hash == NULL) {
        return NULL;
    }

    User *user = (User*)malloc(sizeof(User));
    if (user == NULL) return NULL;

    user->id = id;
    strncpy(user->name, name, MAX_NAME - 1);
    user->name[MAX_NAME - 1] = '\0';
    strncpy(user->phone, phone, MAX_PHONE - 1);
    user->phone[MAX_PHONE - 1] = '\0';
    user->role = role;
    strncpy(user->password_hash, password_hash, MAX_HASH_STRING - 1);
    user->password_hash[MAX_HASH_STRING - 1] = '\0';
    user->active_loans = 0;

    return user;
}

// Cria admin default
User* user_create_default_admin()
{
    return user_create(1, "Admin", "999999999", ROLE_ADMIN, "admin123");
}

// Destrói utilizador
void user_destroy(User *user)
{
    if (user == NULL) return;
    free(user);
}

void user_destroy_callback(void *data)
{
    if (data == NULL) return;
    free(data);
}

// Insere utilizador na árvore
int user_insert(AVLTree *tree, User *user)
{
    if (tree == NULL || user == NULL) return 0;
    return avl_insert(tree, user->id, user);
}

// Procura utilizador
User* user_find(AVLTree *tree, int id)
{
    if (tree == NULL) return NULL;
    return (User*)avl_search(tree, id);
}

// Atualiza utilizador
int user_update(AVLTree *tree, int id, const char *name, const char *phone)
{
    User *user = user_find(tree, id);
    if (user == NULL) return 0;

    if (name != NULL && strlen(name) > 0) {
        strncpy(user->name, name, MAX_NAME - 1);
        user->name[MAX_NAME - 1] = '\0';
    }

    if (phone != NULL && strlen(phone) > 0) {
        strncpy(user->phone, phone, MAX_PHONE - 1);
        user->phone[MAX_PHONE - 1] = '\0';
    }

    return 1;
}

// Remove utilizador
int user_delete(AVLTree *tree, int id)
{
    User *user = user_find(tree, id);
    if (user == NULL) return 0;

    if (!avl_remove(tree, id)) return 0;

    user_destroy(user);
    return 1;
}

// Imprime utilizador
void user_print(const User *user)
{
    if (user == NULL) return;

    printf("\n========== USER ==========\n");
    printf("ID: %d\n", user->id);
    printf("Name: %s\n", user->name);
    printf("Phone: %s\n", user->phone);
    printf("Role: %s\n", user->role == ROLE_ADMIN ? "Admin" : "Student");
    printf("Active Loans: %d\n", user->active_loans);
}

void user_print_callback(void *data)
{
    user_print((User*)data);
}

// Lista todos os utilizadores
void user_list_all(AVLTree *tree)
{
    if (tree == NULL) return;
    printf("\n=== ALL USERS ===\n");
    avl_inorder(tree, user_print_callback);
}

// Verifica se pode fazer empréstimo
int user_can_borrow(const User *user)
{
    if (user == NULL) return 0;
    return user->active_loans < MAX_ACTIVE_LOANS;
}