#ifndef VALIDATION_H
#define VALIDATION_H

#include "types.h"

/* User validation */
int validate_user_id(int id);
int validate_username(const char *username);
int validate_password(const char *password);
int validate_name(const char *name);
int validate_phone(const char *phone);

/* Book validation */
int validate_book_id(int id);
int validate_title(const char *title);
int validate_author(const char *author);
int validate_category(const char *category);
int validate_quantity(int total, int available);

int is_positive_int(int value);

#endif