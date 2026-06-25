#ifndef USER_H
#define USER_H

#include "types.h"
#include "avl.h"


typedef struct {
    int  id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    Role role;
    char password_hash[MAX_HASH_STRING];
    int active_loans;
} User;

// Criação / Destruição

User *user_create_default_admin();

User *user_create(
    int id,
    const char *name,
    const char *phone,
    Role role,
    const char *password
);

void user_destroy(User *user);
void user_destroy_callback(void *data);



//CRUD

int user_insert(
    AVLTree *tree,
    User *user
);

User *user_find(
    AVLTree *tree,
    int id
);

int user_update(
    AVLTree *tree,
    int id,
    const char *name,
    const char *phone
);

int user_delete(
    AVLTree *tree,
    int id
);

//IMPRESSÃO

void user_print(const User *user);

void user_print_callback(void *data);

void user_list_all(AVLTree *tree);

int user_can_borrow(const User *user);

#endif
