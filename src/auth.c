#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"


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


Session auth_login(AVLTree *user_tree, int id, const char *password)
{
    Session session;

    session.authenticated = 0;
    session.user = NULL;


    User *user = (User *) avl_search(user_tree, id);


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