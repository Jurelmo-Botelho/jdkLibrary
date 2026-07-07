#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "avl.h"
#include "loan.h"
#include "date.h"
#include "utils.h"
#include "auth.h"
#include "validation.h"

int user_id_counter = 1;

void create_default_admin(AVLNode **root)
{
    if (root == NULL)
    {
        printf("Erro: arvore de utilizadores invalida.\n");
        return;
    }

    /* verificar se já existe admin (id = 1 fixo) */
    if (user_find(*root, 1) != NULL)
        return;

    User *admin = create_user(
        "admin",
        "admin123",
        "Administrador",
        30,
        "000000000",
        ROLE_ADMIN
    );

    if (admin == NULL)
    {
        printf("Erro: nao foi possivel criar admin padrao.\n");
        return;
    }

    *root = user_insert(*root, admin);

    printf("Admin padrao criado com sucesso.\n");
}

User *create_user(const char *username, const char *password, const char *name, int age, const char *phone, Role role){

    if (!validate_username(username))
        return NULL;

    if (!validate_password(password))
        return NULL;

    if (!validate_name(name))
        return NULL;

    if (age < 1 || age > 120)
    {
        printf("Erro: idade invalida.\n");
        return NULL;
    }

    if (!validate_phone(phone))
        return NULL;

    User *user = (User *)malloc(sizeof(User));

    if (user == NULL)
    {
        printf("Erro: falha na alocação de memória.\n");
        return NULL;
    }

    user->id = user_id_counter++;

    strncpy(user->username, username, MAX_USERNAME);
    strncpy(user->password, password, MAX_PASSWORD);
    strncpy(user->name, name, MAX_NAME);
    user->age = age;
    strncpy(user->phone, phone, MAX_PHONE);

    user->role = role;
    user->activeLoans = 0;

    user->myLoans.head = NULL;       
    user->myLoans.tail = NULL;       
    user->myLoans.quantity = 0;  

    return user;
}

int update_user(AVLNode *root, int id, const char *newName, int newAge, const char *newPhone){

    AVLNode *node = avl_search(root, id);

    if (node == NULL)
    {
        printf("Erro: utilizador nao encontrado.\n");
        return 0;
    }

    User *user = (User *)node->data;

    if (newName != NULL)
    {
        if (!validate_name(newName))
            return 0;

        strncpy(user->name, newName, MAX_NAME - 1);
        user->name[MAX_NAME - 1] = '\0';
    }

    if (newAge > 0)
    {
        if (newAge > 120)
        {
            printf("Erro: idade invalida.\n");
            return 0;
        }

        user->age = newAge;
    }

    if (newPhone != NULL)
    {
        if (!validate_phone(newPhone))
            return 0;

        strncpy(user->phone, newPhone, MAX_PHONE - 1);
        user->phone[MAX_PHONE - 1] = '\0';
    }

    printf("Utilizador atualizado com sucesso.\n");
    return 1;
}

int delete_user(AVLNode **root, int id)
{
    if (root == NULL || *root == NULL)
    {
        printf("Erro: arvore invalida.\n");
        return 0;
    }

    User *user = user_find(*root, id);

    if (user == NULL)
    {
        printf("Erro: utilizador nao encontrado.\n");
        return 0;
    }

    if (user->role == ROLE_ADMIN)
    {
        printf("Erro: nao e permitido remover um administrador.\n");
        return 0;
    }

    *root = avl_remove(*root, id);

    printf("Utilizador removido com sucesso.\n");
    return 1;
}

void print_user(void *data)
{
    if (data == NULL)
    {
        printf("Erro: utilizador inválido.\n");
        return;
    }

    User *user = (User *)data;

    printf("ID: %d | Username: %s | Nome: %s | Idade: %d | Telefone: %s | Emprestimos: %d\n",
           user->id,
           user->username,
           user->name,
           user->age,
           user->phone,
           user->activeLoans);
}

void user_print_all(AVLNode *root)
{
    if (root == NULL)
        return;

    user_print_all(root->left);

    print_user(root->data);

    user_print_all(root->right);
}

void free_user(void *data)
{
    if (data == NULL)
        return;

    User *user = (User *)data;

    Loan *current = user->myLoans.head;
    while (current != NULL) {
        Loan *next = current->nextUser; 
        free(current);                 
        current = next;                 
    }

    free(user); 
}

int user_can_borrow(User *user)
{
    if (user == NULL)
    {
        printf("Erro: utilizador inválido.\n");
        return 0;
    }

    if (user->activeLoans >= MAX_ACTIVE_LOANS)
    {
        printf("Erro: limite de empréstimos atingido.\n");
        return 0;
    }

    return 1;
}

AVLNode *user_insert(AVLNode *root, User *user)
{
    if (user == NULL)
    {
        printf("Erro: utilizador inválido.\n");
        return root;
    }

    root = avl_insert(root, user->id, user);
    return root;
}

AVLNode *user_remove(AVLNode *root, int id)
{
    return avl_remove(root, id);
}

User *user_find(AVLNode *root, int id)
{
    AVLNode *node = avl_search(root, id);

    if (node == NULL)
        return NULL;

    return (User *)node->data;
}

void load_users_from_file(AVLNode **userRoot, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Arquivo %s não encontrado.\n", filename);
        return;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        int id, age, role, activeLoans;
        char username[50], name[100], password[50], phone[20];
        
        sscanf(line, "%d,%49[^,],%99[^,],%49[^,],%19[^,],%d,%d,%d",
               &id, username, name, password, phone, &age, &role, &activeLoans);
        
        User *user = create_user(username, password, name, age, phone, (Role)role);
        if (user) {
            user->id = id;
            user->activeLoans = activeLoans;
            user->myLoans.head = NULL;
            user->myLoans.tail = NULL;
            user->myLoans.quantity = 0;
            if (id >= user_id_counter) {
                user_id_counter = id + 1;
            }
            *userRoot = user_insert(*userRoot, user);
            loaded++;
        }
    }
    
    fclose(file);
    printf("Carregados %d utilizadores do arquivo %s\n", loaded, filename);
}

void save_user_to_file(User *user, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%d\n",
            user->id, user->username, user->name, user->password,
            user->phone, user->age, user->role, user->activeLoans);
    
    fclose(file);
}

void save_users_to_file(AVLNode *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    save_users_recursive(root, file);
    fclose(file);
}

void save_users_recursive(AVLNode *node, FILE *file) {
    if (!node) return;
    
    save_users_recursive(node->left, file);
    
    User *user = (User*)node->data;
    fprintf(file, "%d,%s,%s,%s,%s,%d,%d,%d\n",
            user->id,
            user->username,
            user->name,
            user->password,
            user->phone,
            user->age,
            user->role,
            user->activeLoans);
    
    save_users_recursive(node->right, file);
}