#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "validation.h"


int is_positive_int(int value)
{
    return value > 0;
}

//User

int validate_user_id(int id)
{
    if (!is_positive_int(id))
    {
        printf("Erro: ID de utilizador inválido.\n");
        return 0;
    }
    return 1;
}

int validate_username(const char *username)
{
    if (username == NULL || strlen(username) < 3)
    {
        printf("Erro: username inválido.\n");
        return 0;
    }
    return 1;
}

int validate_password(const char *password)
{
    if (password == NULL || strlen(password) < 4)
    {
        printf("Erro: password invalida.\n");
        return 0;
    }

    return 1;
}

int validate_name(const char *name)
{
    if (name == NULL || strlen(name) < 3)
    {
        printf("Erro: nome inválido.\n");
        return 0;
    }
    return 1;
}

int validate_phone(const char *phone)
{
    if (phone == NULL)
    {
        printf("Erro: telefone inválido.\n");
        return 0;
    }

    int len = strlen(phone);

    if (len < 7 || len > MAX_PHONE)
    {
        printf("Erro: telefone fora do formato.\n");
        return 0;
    }

    return 1;
}

//Book

int validate_book_id(int id)
{
    if (!is_positive_int(id))
    {
        printf("Erro: ID de livro inválido.\n");
        return 0;
    }
    return 1;
}

int validate_title(const char *title)
{
    if (title == NULL || strlen(title) < 1)
    {
        printf("Erro: título inválido.\n");
        return 0;
    }
    return 1;
}

int validate_author(const char *author)
{
    if (author == NULL || strlen(author) < 2)
    {
        printf("Erro: autor inválido.\n");
        return 0;
    }
    return 1;
}

int validate_category(const char *category)
{
    if (category == NULL || strlen(category) < 2)
    {
        printf("Erro: categoria inválida.\n");
        return 0;
    }
    return 1;
}

int validate_quantity(int total, int available)
{
    if (total < 0 || available < 0)
    {
        printf("Erro: quantidades negativas não são permitidas.\n");
        return 0;
    }

    if (available > total)
    {
        printf("Erro: quantidade disponível maior que o total.\n");
        return 0;
    }

    return 1;
}