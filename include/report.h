#ifndef REPORT_H
#define REPORT_H

#include "avl.h"
#include "types.h"
#include "loan.h"

typedef struct {
    Book **mostBorrowed;
    int mostBorrowedCount;
    Book **leastBorrowed;
    int leastBorrowedCount;
    Book **neverBorrowed;
    int neverBorrowedCount;
    Book **available;
    int availableCount;
    Book **unavailable;
    int unavailableCount;
    
    User **mostActive;
    int mostActiveCount;
    User **mostActiveLoans;
    int mostActiveLoansCount;
    User **noLoans;
    int noLoansCount;
    User **overdue;
    int overdueCount;
    
    int totalBooks;
    int totalUsers;
    int totalLoans;
    int totalReturns;
    int totalReservations;
} ReportData;

ReportData* report_generate(AVLNode *bookRoot, AVLNode *userRoot, 
                            LoanList *globalLoans, HistoryList *history);
void report_save_to_file(ReportData *report, const char *filename);
void report_print(ReportData *report);
void report_free(ReportData *report);

#endif