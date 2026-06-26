#ifndef VALIDATION_H
#define VALIDATION_H

#include "types.h"

int validate_user_number(int id);

int validate_username(const char *username);

int validate_name(const char *name);

int validate_phone(const char *phone);

int validate_password(const char *password);

int validate_role(Role role);

#endif