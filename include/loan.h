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

// ========== CRIAÇÃO E DESTRUIÇÃO ==========
LoanList* loan_list_create(void);
void loan_list_destroy(LoanList *list);

// ========== OPERAÇÕES BÁSICAS ==========
int loan_list_add(LoanList *list, Loan *loan);
Loan* loan_list_find_by_id(LoanList *list, int id);

// ========== FUNÇÕES PARA FICHEIROS ==========
Loan* loan_from_string(const char *line);
void loan_to_string(Loan *loan, char *buffer, int buffer_size);
int loan_list_save_to_file(LoanList *list, const char *filename);
int loan_list_load_from_file(LoanList *list, const char *filename);

#endif