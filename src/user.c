#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "validation.h"
#include "auth.h"
#include "avl.h"



User *user_create(int id, const char *name, const char *phone, Role role, const char *password){
    if (
        !validate_user_id(id) || !validate_name(name) ||
        !validate_phone(phone) || !validate_role(role) ||
        !validate_password(password)){
        return NULL;
    }
    User *user = malloc(sizeof(User));

    if (user == NULL){
        return NULL;
    }

    user->id = id;

    strncpy(user->name, name, MAX_NAME - 1);
    user->name[MAX_NAME - 1] = '\0';

    strncpy(user->phone, phone, MAX_PHONE - 1);
    user->phone[MAX_PHONE - 1] = '\0';

    user->role = role;
    user->active_loans = 0;

    unsigned long hash = auth_hash_password(password);

    auth_hash_to_string(
        hash,
        user->password_hash
    );

    return user;
}
User *user_create_default_admin(){
    return user_create(
        1,
        "Admin",
        "999999999",
        ROLE_ADMIN,
        "admin123"
    );
}

void user_destroy(User *user){
    if (user == NULL){
        return;
    }

    free(user);
}


void user_destroy_callback(void *data)
{
    if (data == NULL){
        return;
    }

    free(data);
}



int user_insert(AVLTree *tree, User *user){
    if (tree == NULL || user == NULL){
        return 0;
    }

    return avl_insert(tree, user->id, user);
}


User *user_find(AVLTree *tree, int id){
    if (tree == NULL){
        return NULL;
    }

    return (User *) avl_search(tree, id);
}


int user_update(AVLTree *tree, int id, const char *name, const char *phone){
    User *user = user_find(tree, id);

    if (user == NULL){
        return 0;
    }

    if (name != NULL){

        if (!validate_name(name)){
            return 0;
        }

        strncpy(user->name, name, MAX_NAME - 1);

        user->name[MAX_NAME - 1] = '\0';
    }


    if (phone != NULL){

         if (!validate_phone(phone)){
            return 0;
        }

        strncpy(user->phone, phone, MAX_PHONE - 1);

        user->phone[MAX_PHONE - 1] = '\0';
    }

    return 1;
}


int user_delete(AVLTree *tree, int id){
    User *user = user_find(tree, id);

    if (user == NULL){
        return 0;
    }

    user_destroy(user);

    return avl_remove(tree, id);
}


void user_print(const User *user){
    if (user == NULL){
        return;
    }

    printf(
        "ID: %d\n"
        "Nome: %s\n"
        "Telefone: %s\n"
        "Emprestimos Ativos: %d\n",
        user->id,
        user->name,
        user->phone,
        user->active_loans
    );

    if (user->role == ROLE_ADMIN){
        printf("Papel: Administrador\n");
    }else{
        printf("Papel: Estudante\n");
    }

    printf("\n");
}



void user_print_callback(void *data)
{
    user_print((User *) data);
}


void user_list_all(AVLTree *tree){
    if (tree == NULL){
        return;
    }

    avl_inorder(tree, user_print_callback);
}


int user_can_borrow(const User *user){
    if (user == NULL){
        return 0;
    }

    return (user->active_loans < MAX_ACTIVE_LOANS);
}