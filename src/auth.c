#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "avl.h"


static const char *search_username;
static User *found_user;


void search_username_callback(void *data)
{
    User *u = (User*)data;

    if(strcmp(u->username, search_username) == 0)
    {
        found_user = u;
    }
}

unsigned long auth_hash_password(const char *password)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *password++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}


void auth_hash_to_string(unsigned long hash, char *buf)
{
    snprintf(buf, MAX_HASH_STRING, "%lu", hash);
}

User* user_find_by_username(AVLTree *tree, const char *username)
{
    if(tree == NULL || username == NULL)
        return NULL;


    search_username = username;
    found_user = NULL;


    avl_inorder(tree, search_username_callback);


    return found_user;
}

Session auth_login(AVLTree *user_tree, const char *username, const char *password)
{
    Session session;

    session.authenticated = 0;
    session.user = NULL;


    User *user = user_find_by_username(user_tree, username);


    if (user == NULL)
    {
        return session;
    }


    unsigned long hash = auth_hash_password(password);

    char hash_string[MAX_HASH_STRING];

    auth_hash_to_string(hash, hash_string);


    if (strcmp(hash_string, user->password_hash) != 0)
    {
        return session;
    }


    session.authenticated = 1;
    session.user = user;


    return session;
}


void auth_logout(Session *session)
{
    if (session == NULL)
    {
        return;
    }


    session->authenticated = 0;
    session->user = NULL;
}


int auth_has_permission(Session *session, Role minimum_role)
{
    if (session == NULL || !session->authenticated)
    {
        return 0;
    }


    return session->user->role <= minimum_role;
}