#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"
#include "validation.h"
#include "user.h"
#include "book.h"

Session *create_session(void)
{
    Session *session = (Session *)malloc(sizeof(Session));

    if (session == NULL)
    {
        printf("Erro: falha ao criar sessão.\n");
        return NULL;
    }

    session->currentUser = NULL;
    session->isLoggedIn = 0;

    return session;
}

void destroy_session(Session *session)
{
    if (session == NULL)
        return;

    free(session);
}

void logout(Session *session)
{
    if (session == NULL)
    {
        printf("Erro: sessão inválida.\n");
        return;
    }

    session->currentUser = NULL;
    session->isLoggedIn = 0;

    printf("Logout efetuado com sucesso.\n");
}

User *find_user_by_username(AVLNode *root, const char *username)
{
    if (root == NULL || username == NULL)
        return NULL;

    /* percurso simples in-order */
    if (root->left != NULL)
    {
        User *leftResult = find_user_by_username(root->left, username);
        if (leftResult != NULL)
            return leftResult;
    }

    User *user = (User *)root->data;

    if (strcmp(user->username, username) == 0)
        return user;

    if (root->right != NULL)
        return find_user_by_username(root->right, username);

    return NULL;
}

int login(Session *session, AVLNode *userRoot, const char *username, const char *password)
{
    if (session == NULL)
    {
        printf("Erro: sessão inválida.\n");
        return 0;
    }

    User *user = find_user_by_username(userRoot, username);

    if (user == NULL)
    {
        printf("Erro: utilizador não encontrado.\n");
        return 0;
    }

    if (strcmp(user->password, password) != 0)
    {
        printf("Erro: password incorreta.\n");
        return 0;
    }

    session->currentUser = user;
    session->isLoggedIn = 1;

    printf("Login efetuado com sucesso. Bem-vindo %s!\n", user->name);

    return 1;
}

User *register_user(AVLNode **userRoot, const char *username, const char *password, const char *name, int age, const char *phone, Role role){

    if (userRoot == NULL)
    {
        printf("Erro: árvore de utilizadores inválida.\n");
        return NULL;
    }

    if (find_user_by_username(*userRoot, username) != NULL)
    {
        printf("Erro: username já existe.\n");
        return NULL;
    }

    User *newUser = create_user(username, password, name, age, phone, role);

    if (newUser == NULL)
        return NULL;

    *userRoot = user_insert(*userRoot, newUser);

   // save_user_to_file(newUser, "data/users.txt");

    printf("Utilizador registado com sucesso.\n");

    return newUser;
}

Book *register_book(AVLNode **bookRoot, const char *title, const char *author, const char *publisher, const char *category, int year, int minAge, int qty){
    
    if (bookRoot == NULL)
    {
        printf("Erro: arvore de livros invalida.\n");
        return NULL;
    }

    Book *book = create_book(title, author, publisher, category, year, minAge, qty);

    if (book == NULL)
        return NULL;

    *bookRoot = book_insert(*bookRoot, book);

    //save_book_to_file(book, "data/books.txt");

    printf("Livro registado com sucesso.\n");

    return book;
}