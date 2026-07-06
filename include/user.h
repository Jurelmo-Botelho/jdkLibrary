#ifndef USER_H
#define USER_H

#include "types.h"
#include "avl.h"
#include "loan.h" 

typedef struct User
{
    int id;
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];
    char name[MAX_NAME];
    int age;
    char phone[MAX_PHONE];

    Role role;

    int activeLoans;
    LoanList myLoans; 

} User;


void create_default_admin(AVLNode **root);

User *create_user(
    const char *username, 
    const char *password, 
    const char *name, 
    int age, 
    const char *phone, 
    Role role
);

int update_user(
    AVLNode *root,
    int id,
    const char *newName,
    int newAge,
    const char *newPhone
);

int delete_user(AVLNode **root, int id);

void print_user(void *data);
void user_print_all(AVLNode *root);

void free_user(void *data);

AVLNode *user_insert(AVLNode *root, User *user);

AVLNode *user_remove(AVLNode *root, int id);

User *user_find(AVLNode *root,int id);

//Regras de Negócio

int user_can_borrow(User *user);

#endif