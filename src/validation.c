#include <string.h>
#include <ctype.h>

#include "types.h"
#include "validation.h"


int validate_user_id(int id)
{
    return id > 0;
}

int validate_name(const char *name)
{
    if (name == NULL)
    {
        return 0;
    }

    size_t len = strlen(name);

    if (len < 3)
    {
        return 0;
    }

    return 1;
}

int validate_phone(const char *phone)
{
    if (phone == NULL)
    {
        return 0;
    }


    if (strlen(phone) != 9)
    {
        return 0;
    }


    for (int i = 0; phone[i] != '\0'; i++)
    {
        if (!isdigit(phone[i]))
        {
            return 0;
        }
    }


    return 1;
}


int validate_password(const char *password)
{
    if (password == NULL)
    {
        return 0;
    }


    int has_letter = 0;
    int has_digit = 0;


    if (strlen(password) < 6)
    {
        return 0;
    }


    for (int i = 0; password[i] != '\0'; i++)
    {
        if (isalpha(password[i]))
        {
            has_letter = 1;
        }


        if (isdigit(password[i]))
        {
            has_digit = 1;
        }
    }


    return has_letter && has_digit;
}


int validate_role(Role role)
{
    return role == ROLE_ADMIN || role == ROLE_STUDENT;
}