#ifndef LOAN_H
#define LOAN_H

#include "types.h"
#include "date.h"

struct Loan {
    int id;
    Book *book;          
    User *leitor;        

    Date DataLoan;
    Date DateExpected;
    Date DateReturn;
    LoanStatus estado;

    struct Loan *nextGlobal; 
    struct Loan *nextUser;   
};

typedef struct {
    Loan *head;
    Loan *tail;
    int quantity;
} LoanList;

#endif
