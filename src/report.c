#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "report.h"
#include "user.h"
#include "book.h"
#include "loan.h"
#include "reserve.h"
#include "date.h"

static int compare_books_by_borrowed(const void *a, const void *b) {
    Book *bookA = *(Book**)a;
    Book *bookB = *(Book**)b;
    return bookB->timesBorrowed - bookA->timesBorrowed;
}

static int compare_users_by_activity(const void *a, const void *b) {
    User *userA = *(User**)a;
    User *userB = *(User**)b;
    int totalA = userA->activeLoans;
    int totalB = userB->activeLoans;
    return totalB - totalA;
}

static int count_avl_nodes(AVLNode *root) {
    if (!root) return 0;
    return 1 + count_avl_nodes(root->left) + count_avl_nodes(root->right);
}

static void collect_books(AVLNode *node, Book ***books, int *count) {
    if (!node) return;
    collect_books(node->left, books, count);
    Book *book = (Book*)node->data;
    (*books)[(*count)++] = book;
    collect_books(node->right, books, count);
}

static void collect_users(AVLNode *node, User ***users, int *count) {
    if (!node) return;
    collect_users(node->left, users, count);
    User *user = (User*)node->data;
    (*users)[(*count)++] = user;
    collect_users(node->right, users, count);
}

static int count_total_reservations(AVLNode *bookRoot) {
    if (!bookRoot) return 0;
    int total = 0;
    Book *book = (Book*)bookRoot->data;
    if (book->reservations) {
        total += book->reservations->size;
    }
    total += count_total_reservations(bookRoot->left);
    total += count_total_reservations(bookRoot->right);
    return total;
}

static int is_user_overdue(User *user, LoanList *globalLoans) {
    if (!user || !globalLoans) return 0;
    Loan *current = globalLoans->head;
    Date today = date_today();
    while (current) {
        if (current->leitor->id == user->id && 
            current->status == LOAN_ACTIVE &&
            date_compare(today, current->DateExpected) > 0) {
            return 1;
        }
        current = current->nextGlobal;
    }
    return 0;
}

ReportData* report_generate(AVLNode *bookRoot, AVLNode *userRoot, 
                            LoanList *globalLoans, HistoryList *history) {
    ReportData *report = (ReportData*)malloc(sizeof(ReportData));
    if (!report) return NULL;
    
    memset(report, 0, sizeof(ReportData));
    
    int totalBooks = count_avl_nodes(bookRoot);
    int totalUsers = count_avl_nodes(userRoot);
    
    if (totalBooks > 0) {
        Book **allBooks = (Book**)malloc(totalBooks * sizeof(Book*));
        int count = 0;
        collect_books(bookRoot, &allBooks, &count);
        
        Book **borrowed = (Book**)malloc(totalBooks * sizeof(Book*));
        Book **never = (Book**)malloc(totalBooks * sizeof(Book*));
        Book **avail = (Book**)malloc(totalBooks * sizeof(Book*));
        Book **unavail = (Book**)malloc(totalBooks * sizeof(Book*));
        
        int borrowedCount = 0, neverCount = 0, availCount = 0, unavailCount = 0;
        
        for (int i = 0; i < totalBooks; i++) {
            if (allBooks[i]->timesBorrowed > 0) {
                borrowed[borrowedCount++] = allBooks[i];
            } else {
                never[neverCount++] = allBooks[i];
            }
            
            if (allBooks[i]->availableQuantity > 0) {
                avail[availCount++] = allBooks[i];
            } else {
                unavail[unavailCount++] = allBooks[i];
            }
        }
        
        qsort(borrowed, borrowedCount, sizeof(Book*), compare_books_by_borrowed);
        
        int topCount = borrowedCount < 10 ? borrowedCount : 10;
        report->mostBorrowed = (Book**)malloc(topCount * sizeof(Book*));
        for (int i = 0; i < topCount; i++) {
            report->mostBorrowed[i] = borrowed[i];
        }
        report->mostBorrowedCount = topCount;
        
        int bottomCount = borrowedCount < 10 ? borrowedCount : 10;
        report->leastBorrowed = (Book**)malloc(bottomCount * sizeof(Book*));
        for (int i = 0; i < bottomCount; i++) {
            report->leastBorrowed[i] = borrowed[borrowedCount - 1 - i];
        }
        report->leastBorrowedCount = bottomCount;
        
        report->neverBorrowed = never;
        report->neverBorrowedCount = neverCount;
        
        report->available = avail;
        report->availableCount = availCount;
        
        report->unavailable = unavail;
        report->unavailableCount = unavailCount;
        
        free(allBooks);
        free(borrowed);
    }
    
    if (totalUsers > 0) {
        User **allUsers = (User**)malloc(totalUsers * sizeof(User*));
        int count = 0;
        collect_users(userRoot, &allUsers, &count);
        
        User **active = (User**)malloc(totalUsers * sizeof(User*));
        User **activeLoans = (User**)malloc(totalUsers * sizeof(User*));
        User **noLoans = (User**)malloc(totalUsers * sizeof(User*));
        User **overdue = (User**)malloc(totalUsers * sizeof(User*));
        
        int activeCount = 0, activeLoansCount = 0, noLoansCount = 0, overdueCount = 0;
        
        for (int i = 0; i < totalUsers; i++) {
            if (allUsers[i]->activeLoans > 0) {
                active[activeCount++] = allUsers[i];
                activeLoans[activeLoansCount++] = allUsers[i];
            } else {
                noLoans[noLoansCount++] = allUsers[i];
            }
            
            if (is_user_overdue(allUsers[i], globalLoans)) {
                overdue[overdueCount++] = allUsers[i];
            }
        }
        
        qsort(active, activeCount, sizeof(User*), compare_users_by_activity);
        qsort(activeLoans, activeLoansCount, sizeof(User*), compare_users_by_activity);
        
        int topCount = activeCount < 10 ? activeCount : 10;
        report->mostActive = (User**)malloc(topCount * sizeof(User*));
        for (int i = 0; i < topCount; i++) {
            report->mostActive[i] = active[i];
        }
        report->mostActiveCount = topCount;
        
        int topLoansCount = activeLoansCount < 10 ? activeLoansCount : 10;
        report->mostActiveLoans = (User**)malloc(topLoansCount * sizeof(User*));
        for (int i = 0; i < topLoansCount; i++) {
            report->mostActiveLoans[i] = activeLoans[i];
        }
        report->mostActiveLoansCount = topLoansCount;
        
        report->noLoans = noLoans;
        report->noLoansCount = noLoansCount;
        
        report->overdue = overdue;
        report->overdueCount = overdueCount;
        
        free(allUsers);
        free(active);
        free(activeLoans);
    }
    
    report->totalBooks = totalBooks;
    report->totalUsers = totalUsers;
    report->totalLoans = globalLoans ? globalLoans->quantity : 0;
    report->totalLoans += history ? history->quantity : 0;
    report->totalReturns = history ? history->quantity : 0;
    report->totalReservations = count_total_reservations(bookRoot);
    
    return report;
}

void report_print(ReportData *report) {
    if (!report) return;
    
    printf("\n======== Relatorio da Biblioteca =========\n\n");
    
    printf("Livros:\n");
    printf("Mais Emprestados:\n");
    for (int i = 0; i < report->mostBorrowedCount; i++) {
        printf("  %d. %s - %d emprestimos\n", i+1, 
               report->mostBorrowed[i]->title, 
               report->mostBorrowed[i]->timesBorrowed);
    }
    
    printf("\nMenos EMprestados:\n");
    for (int i = 0; i < report->leastBorrowedCount; i++) {
        printf("  %d. %s - %d emprestimos\n", i+1,
               report->leastBorrowed[i]->title,
               report->leastBorrowed[i]->timesBorrowed);
    }
    
    printf("\n Nunca EMprestados  (%d):\n", report->neverBorrowedCount);
    for (int i = 0; i < report->neverBorrowedCount && i < 10; i++) {
        printf("  - %s\n", report->neverBorrowed[i]->title);
    }
    if (report->neverBorrowedCount > 10) {
        printf("  ... e %d mais\n", report->neverBorrowedCount - 10);
    }
    
    printf("\n Disponiveis: %d\n", report->availableCount);
    printf(" Indisponíveis: %d\n", report->unavailableCount);
    
    printf("\n Leitores :\n");
    printf(" Mais Activos:\n");
    for (int i = 0; i < report->mostActiveCount; i++) {
        printf("  %d. %s - %d active loans\n", i+1,
               report->mostActive[i]->name,
               report->mostActive[i]->activeLoans);
    }
    
    printf("\n Com empréstimos mais activos: \n");
    for (int i = 0; i < report->mostActiveLoansCount; i++) {
        printf("  %d. %s - %d loans\n", i+1,
               report->mostActiveLoans[i]->name,
               report->mostActiveLoans[i]->activeLoans);
    }
    
    printf("\n Sem empréstimos: %d\n", report->noLoansCount);
    printf(" Passaram da Data: %d\n", report->overdueCount);
    
    printf("\n Totais: :\n");
    printf("Livros: %d\n", report->totalBooks);
    printf("Leitores: %d\n", report->totalUsers);
    printf("Empréstimos: %d\n", report->totalLoans);
    printf("Devoluções: %d\n", report->totalReturns);
    printf("Reservas: %d\n", report->totalReservations);
}

void report_save_to_file(ReportData *report, const char *filename) {
    if (!report) return;
    
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir ficheiro: %s\n", filename);
        return;
    }
    
    fprintf(file, "Relatorio  \n\n");
    
    fprintf(file, "Livros:\n");
    fprintf(file, "Mias Emprestados:\n");
    for (int i = 0; i < report->mostBorrowedCount; i++) {
        fprintf(file, "%d. %s - %d borrows\n", i+1,
                report->mostBorrowed[i]->title,
                report->mostBorrowed[i]->timesBorrowed);
    }
    
    fprintf(file, "\nMenos Emprestados:\n");
    for (int i = 0; i < report->leastBorrowedCount; i++) {
        fprintf(file, "%d. %s - %d borrows\n", i+1,
                report->leastBorrowed[i]->title,
                report->leastBorrowed[i]->timesBorrowed);
    }
    
    fprintf(file, "\nNunca Emprestados (%d):\n", report->neverBorrowedCount);
    for (int i = 0; i < report->neverBorrowedCount && i < 10; i++) {
        fprintf(file, "- %s\n", report->neverBorrowed[i]->title);
    }
    if (report->neverBorrowedCount > 10) {
        fprintf(file, "... e %d mais\n", report->neverBorrowedCount - 10);
    }
    
    fprintf(file, "\n Disponiveis: %d\n", report->availableCount);
    fprintf(file, "Indisponiveis: %d\n", report->unavailableCount);
    
    fprintf(file, "\n Leitores :\n");
    fprintf(file, "Mais Activos:\n");
    for (int i = 0; i < report->mostActiveCount; i++) {
        fprintf(file, "%d. %s - %d empréstimos activos\n", i+1,
                report->mostActive[i]->name,
                report->mostActive[i]->activeLoans);
    }
    
    fprintf(file, "\n Com mais empréstimos activos:\n");
    for (int i = 0; i < report->mostActiveLoansCount; i++) {
        fprintf(file, "%d. %s - %d loans\n", i+1,
                report->mostActiveLoans[i]->name,
                report->mostActiveLoans[i]->activeLoans);
    }
    
    fprintf(file, "\n Sem EMpréstimos: %d\n", report->noLoansCount);
    fprintf(file, " Passaram o praso: %d\n", report->overdueCount);
    
    fprintf(file, "\nTotais:\n");
    fprintf(file, "Livros: %d\n", report->totalBooks);
    fprintf(file, "Leitores: %d\n", report->totalUsers);
    fprintf(file, "Empréstimos: %d\n", report->totalLoans);
    fprintf(file, "Devoluções: %d\n", report->totalReturns);
    fprintf(file, "Reservas: %d\n", report->totalReservations);
    
    fclose(file);
    printf("Relatorio salvo em: %s\n", filename);
}

void report_free(ReportData *report) {
    if (!report) return;
    
    free(report->mostBorrowed);
    free(report->leastBorrowed);
    free(report->neverBorrowed);
    free(report->available);
    free(report->unavailable);
    free(report->mostActive);
    free(report->mostActiveLoans);
    free(report->noLoans);
    free(report->overdue);
    
    free(report);
}