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
    LoanStatus status;

    struct Loan *nextGlobal; 
    struct Loan *nextUser;   
};

typedef struct {
    Loan *head;
    Loan *tail;
    int quantity;
} LoanList;

typedef struct {
    Loan *head;
    Loan *tail;
    int quantity;
} HistoryList;

Loan* loan_create(int id, Book *book, User *user, Date dataEmprestimo, Date dataPrevista);
void loan_add_to_global(LoanList *list, Loan *loan);
void loan_add_to_user(User *user, Loan *loan);
Loan* loan_find_by_user_and_book(LoanList *list, int userId, int bookId);
void loan_print_user_loans(User *user);
void loan_print_all_active(LoanList *globalList);
void loan_return_book(LoanList *globalList, HistoryList *history, Loan *loan);
void loan_add_to_history(HistoryList *history, Loan *loan);
void process_book_return(LoanList *globalList, HistoryList *history, AVLNode *bookRoot, int bookId, int userId);

/*
// Funções de Criação
void loan_add_to_history(HistoryList *history, Loan *loan);

// Funções de Validação
bool loan_can_borrow(User *user, Book *book);
bool loan_validate_age(User *user, Book *book);

// Funções de Busca
Loan* loan_find_by_id(LoanList *list, int id);
Loan* loan_find_by_user_and_book(LoanList *list, int userId, int bookId);
LoanList* loan_get_user_loans(User *user);

// Funções de Atualização
void loan_return_book(LoanList *globalList, HistoryList *history, Loan *loan);
void loan_update_status(Loan *loan, LoanStatus newStatus);

// Funções de Limpeza
void loan_free_global(LoanList *list);
void loan_free_user_list(User *user);
void loan_free_history(HistoryList *history);
*/

#endif
