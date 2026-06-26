#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "user.h"
#include "auth.h"
#include "avl.h"
#include "types.h"

static const char *search_username = NULL;
static int username_found = 0;

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

//Normalização de username
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void normalize_username(char *username)
{
    if (username == NULL) return;

    //remover espaços início/fim primeiro (trim)
    char *start = username;
    while (isspace((unsigned char)*start)) start++;

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    *(end + 1) = '\0';

    if (start != username)
        memmove(username, start, strlen(start) + 1);

    //remover espaços do meio + lowercase
    int i = 0;  // leitura
    int j = 0;  // escrita

    while (username[i] != '\0')
    {
        if (!isspace((unsigned char)username[i]))
        {
            username[j++] = (char)tolower((unsigned char)username[i]);
        }
        i++;
    }

    username[j] = '\0';
}

User* user_create(int id, const char *username, const char *name, const char *phone, Role role, const char *password){
    if (password == NULL) return NULL;

    if (id == 0) {
        id = user_get_next_id();
    }

    if (id <= 0) return NULL;

    unsigned long hash = auth_hash_password(password);
    char hash_string[MAX_HASH_STRING];
    auth_hash_to_string(hash, hash_string);

    return user_create_with_hash(id, username, name, phone, role, hash_string);
}

// Cria utilizador com hash já calculado
User* user_create_with_hash(int id, const char *username, const char *name, const char *phone,
                            Role role, const char *password_hash)
{
    if (id <= 0 || name == NULL || phone == NULL || password_hash == NULL) {
        return NULL;
    }

    User *user = (User*)malloc(sizeof(User));
    if (user == NULL) return NULL;

    user->id = id;
    strncpy(user->username, username, MAX_USERNAME-1);
    user->username[MAX_USERNAME-1]='\0';
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
    return user_create(1,"admin", "Administrador", "999999999", ROLE_ADMIN, "admin123");
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

//Atualiza Username
int user_update_username(AVLTree *tree, int id, const char *new_username)
{
    if (tree == NULL || new_username == NULL) return 0;

    char temp[MAX_USERNAME];
    strncpy(temp, new_username, MAX_USERNAME);
    temp[MAX_USERNAME - 1] = '\0';

    normalize_username(temp);

    User *user = user_find(tree, id);
    if (user == NULL) return 0;

    // se for igual ao atual, aceita
    if (strcmp(user->username, temp) == 0)
        return 1;

    // verificar duplicação
    if (user_username_exists(tree, temp))
        return 0;

    strncpy(user->username, temp, MAX_USERNAME - 1);
    user->username[MAX_USERNAME - 1] = '\0';

    return 1;
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

//Atualiza Password
int user_update_password(AVLTree *tree, int id, const char *new_password)
{
    if (tree == NULL || new_password == NULL) return 0;

    if (strlen(new_password) < 4)
        return 0;

    User *user = user_find(tree, id);
    if (user == NULL) return 0;

    unsigned long hash = auth_hash_password(new_password);

    char hash_string[MAX_HASH_STRING];
    auth_hash_to_string(hash, hash_string);

    strncpy(user->password_hash, hash_string, MAX_HASH_STRING - 1);
    user->password_hash[MAX_HASH_STRING - 1] = '\0';

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
    printf("Username: %s\n", user->username);
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

//Verificar existência de um username
void username_search_callback(void *data)
{
    User *user = (User*)data;

    if (search_username != NULL &&
        strcmp(user->username, search_username) == 0)
    {
        username_found = 1;
    }
}

int user_username_exists(AVLTree *tree, const char *username)
{
    search_username = username;
    username_found = 0;

    avl_inorder(tree, username_search_callback);

    return username_found;
}