#ifndef BOOK_H
#define BOOK_H

#include "types.h"
#include "avl.h"
#include "reserve.h"
#include <stdio.h>  

extern int book_id_counter;

typedef struct Book{

    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char publisher[MAX_PUBLISHER];
    char category[MAX_CATEGORY];
    int year;
    int minAge;
    int totalQuantity;
    int availableQuantity;
    int timesBorrowed;
    ReserveQueue *reservations;

} Book;


Book *create_book(
    const char *title,
    const char *author,
    const char *publisher,
    const char *category,
    int year,
    int minAge,
    int totalQuantity
);

int update_book(
    AVLNode *root,
    int id,
    const char *newTitle,
    const char *newAuthor,
    const char *newCategory,
    int newMinAge
);

int delete_book(AVLNode **root, int id);

void print_book(void *data);
void book_print_all(AVLNode *root);
void free_book(void *data);

AVLNode *book_insert(AVLNode *root, Book *book);
AVLNode *book_remove(AVLNode *root, int id);
Book *book_find(AVLNode *root, int id);

int book_can_borrow(Book *book, int userAge);
int book_decrease_available(Book *book);
int book_increase_available(Book *book);
void book_print_unavailable(AVLNode *root);
void load_books_from_file(AVLNode **bookRoot, const char *filename);
void save_book_to_file(Book *book, const char *filename);
void save_books_to_file(AVLNode *root, const char *filename);
void save_books_recursive(AVLNode *node, FILE *file);

#endif 