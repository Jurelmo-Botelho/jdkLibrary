#ifndef BOOK_H
#define BOOK_H

#include "types.h"

typedef struct {
    int  code;                      
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char publisher[MAX_PUBLISHER];
    int  year;
    char category[MAX_CATEGORY];
    int  total_copies;
    int  available_copies;
    int  total_loans;             
} Book;

#endif 
