#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"

Book* book_create(int code, const char *title, const char *author,
                  const char *publisher, int year, const char *category,
                  int total_copies)
{
    Book *book = (Book*)malloc(sizeof(Book));
    if (book == NULL) return NULL;

    book->code = code;
    strcpy(book->title, title);
    strcpy(book->author, author);
    strcpy(book->publisher, publisher);
    book->year = year;
    strcpy(book->category, category);
    book->total_copies = total_copies;
    book->available_copies = total_copies;
    book->total_loans = 0;

    return book;
}

// Valida os dados do livro
int book_validate(Book *book)
{
    if (book == NULL) return 0;
    if (book->code <= 0) return 0;
    if (strlen(book->title) == 0) return 0;
    if (strlen(book->author) == 0) return 0;
    if (strlen(book->publisher) == 0) return 0;
    if (book->year < 1500 || book->year > 2100) return 0;
    if (book->total_copies < 0) return 0;
    if (book->available_copies < 0) return 0;
    if (book->available_copies > book->total_copies) return 0;

    return 1;
}


int book_is_available(Book *book)
{
    if (book == NULL) return 0;
    return book->available_copies > 0;
}


int book_loan(Book *book)
{
    if (book == NULL) return 0;
    if (book->available_copies <= 0) return 0;

    book->available_copies--;
    book->total_loans++;
    return 1;
}

// Devolve um livro
int book_return(Book *book)
{
    if (book == NULL) return 0;
    if (book->available_copies >= book->total_copies) return 0;

    book->available_copies++;
    return 1;
}

// Atualiza dados do livro
void book_update(Book *book, const char *title, const char *author,
                 const char *publisher, int year, const char *category)
{
    if (book == NULL) return;

    if (strlen(title) > 0) strcpy(book->title, title);
    if (strlen(author) > 0) strcpy(book->author, author);
    if (strlen(publisher) > 0) strcpy(book->publisher, publisher);
    if (year >= 1500 && year <= 2100) book->year = year;
    if (strlen(category) > 0) strcpy(book->category, category);
}

// Imprime livro
void book_print(Book *book)
{
    if (book == NULL) return;

    printf("\n========== BOOK ==========\n");
    printf("Code: %d\n", book->code);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Publisher: %s\n", book->publisher);
    printf("Year: %d\n", book->year);
    printf("Category: %s\n", book->category);
    printf("Available: %d/%d\n", book->available_copies, book->total_copies);
    printf("Total loans: %d\n", book->total_loans);
}

// Cria livro a partir de uma linha do ficheiro
Book* book_from_string(const char *line)
{
    if (line == NULL) return NULL;

    int code, year, total_copies, available_copies, total_loans;
    char title[MAX_TITLE], author[MAX_AUTHOR], publisher[MAX_PUBLISHER];
    char category[MAX_CATEGORY];

    int result = sscanf(line, "%d,%[^,],%[^,],%[^,],%d,%[^,],%d,%d,%d",
                        &code, title, author, publisher, &year, category,
                        &total_copies, &available_copies, &total_loans);

    if (result != 9) return NULL;

    Book *book = book_create(code, title, author, publisher, year, category, total_copies);
    if (book == NULL) return NULL;

    book->available_copies = available_copies;
    book->total_loans = total_loans;

    return book;
}

// Converte livro para string (para guardar no ficheiro)
void book_to_string(Book *book, char *buffer, int buffer_size)
{
    if (book == NULL || buffer == NULL) return;

    snprintf(buffer, buffer_size,
             "%d,%s,%s,%s,%d,%s,%d,%d,%d",
             book->code, book->title, book->author, book->publisher,
             book->year, book->category, book->total_copies,
             book->available_copies, book->total_loans);
}