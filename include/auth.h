#ifndef AUTH_H
#define AUTH_H

#include "types.h"
#include "avl.h"
#include "user.h"
#include "book.h"


typedef struct Session
{
    User *currentUser;
    int isLoggedIn;

} Session;


Session *create_session(void);
void destroy_session(Session *session);

int login(
    Session *session, 
    AVLNode *userRoot, 
    const char *username, 
    const char *password
);

void logout(Session *session);

User *register_user(
    AVLNode **userRoot, 
    const char *username, 
    const char *password, 
    const char *name, 
    int age, 
    const char *phone, 
    Role role
);

Book *register_book(
    AVLNode **bookRoot,
    const char *title,
    const char *author,
    const char *publisher,
    const char *category,
    int year,
    int minAge,
    int qty
);

User *find_user_by_username(AVLNode *root, const char *username);

#endif