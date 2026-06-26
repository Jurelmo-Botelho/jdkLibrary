#ifndef AUTH_H
#define AUTH_H

#include "user.h"
#include "avl.h"

typedef struct {
    int authenticated;
    User *user;
} Session;

// Hash
unsigned long auth_hash_password(const char *password);
void auth_hash_to_string(unsigned long hash, char *buf);

// Login/Logout
Session auth_login(AVLTree *user_tree, const char *username, const char *password);
void auth_logout(Session *session);

// Permissões 
int auth_has_permission(Session *session, Role minimum_role);

#endif