#ifndef LOAN_H
#define LOAN_H

#include "types.h"

typedef struct {
    int id;                 
    int book_code;          
    int user_number;         
    Date loan_date;
    Date due_date;           
    Date return_date;        
    LoanStatus status;
} Loan;

typedef struct LoanNode {
    Loan loan;
    struct LoanNode *next;
} LoanNode;

typedef struct {
    LoanNode *head;
    int       total;        
    int       next_id;  
} LoanList;

#endif 
