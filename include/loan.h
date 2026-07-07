#ifndef LOAN_H
#define LOAN_H

#include "types.h"
#include "date.h"
#include "avl.h"
#include <stdio.h> 

extern int loan_id_counter; 

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
void loan_free_global(LoanList *list);
void loan_free_user_list(User *user);
void loan_free_history(HistoryList *history);
Loan* loan_find_by_id(LoanList *globalList, int id);
Loan* loan_find_by_book_id(LoanList *globalList, int bookId);
Loan* loan_find_by_user_id(LoanList *globalList, int userId);
void loan_list_active_for_return(LoanList *globalList);

void load_loans_from_file(AVLNode *bookRoot, AVLNode *userRoot, 
                          LoanList **globalList, const char *filename);
void load_history_from_file(AVLNode *bookRoot, AVLNode *userRoot, 
                            HistoryList **history, const char *filename);
void save_loans_to_file(LoanList *list, const char *filename);
void save_history_to_file(HistoryList *history, const char *filename);

#endif