#ifndef AUTH_H
#define AUTH_H

#include "user.h"
#include "avl.h"


typedef struct {
    int   authenticated;
    User *user;
} Session;


Session auth_login(AVLTree *user_tree, int id, const char *password);
void  auth_logout(Session *session);
int  auth_has_permission(Session *session, Role minimum_role);
unsigned long auth_hash_password(const char *password);
void auth_hash_to_string(unsigned long hash, char *buf);

#endif 
