#include "data_persistence.h"
#include "user.h"
#include "book.h"
#include "loan.h"
#include "reserve.h"
#include "report.h"
#include <stdio.h>

void load_all_data(AVLNode **userRoot, AVLNode **bookRoot, 
                   LoanList **globalLoans, HistoryList **history) {
    printf("\n CARREGANDO DADOS \n");
    
    load_users_from_file(userRoot, "data/users.txt");
    load_books_from_file(bookRoot, "data/books.txt");
    
    load_loans_from_file(*bookRoot, *userRoot, globalLoans, "data/loans.txt");
    load_history_from_file(*bookRoot, *userRoot, history, "data/history.txt");
    load_reservations_from_file(*bookRoot, *userRoot, "data/reservations.txt");
    
    printf("DADOS CARREGADOS\n\n");
}

void save_all_data(AVLNode *userRoot, AVLNode *bookRoot, 
                   LoanList *globalLoans, HistoryList *history) {
    printf("\nSALVANDO DADOS\n");
    
    save_users_to_file(userRoot, "data/users.txt");
    save_books_to_file(bookRoot, "data/books.txt");
    save_loans_to_file(globalLoans, "data/loans.txt");
    save_history_to_file(history, "data/history.txt");
    save_reservations_to_file(bookRoot, "data/reservations.txt");
    

    ReportData *report = report_generate(bookRoot, userRoot, globalLoans, history);
    if (report) {
        report_save_to_file(report, "data/report.txt");
        report_free(report);
    }

    printf("Dados salvos com sucesso.\n");
}