#ifndef BOOK_H
#define BOOK_H

#include "types.h"

typedef struct {
    int code;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char publisher[MAX_PUBLISHER];
    int year;
    char category[MAX_CATEGORY];
    int total_copies;
    int available_copies;
    int total_loans;
} Book;

// Criação
Book* book_create(int code, const char *title, const char *author,
                  const char *publisher, int year, const char *category,
                  int total_copies);

// Validação
int book_validate(Book *book);

// Operações
int book_is_available(Book *book);
int book_loan(Book *book);
int book_return(Book *book);

// Atualização
void book_update(Book *book, const char *title, const char *author,
                 const char *publisher, int year, const char *category);

// Impressão
void book_print(Book *book);

// Funções para ficheiros
Book* book_from_string(const char *line);
void book_to_string(Book *book, char *buffer, int buffer_size);

#endif