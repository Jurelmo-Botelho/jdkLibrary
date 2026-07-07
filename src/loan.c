#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loan.h"
#include "user.h"
#include "book.h"
#include "reserve.h"
#include "date.h"
#include "utils.h" 
#include "auth.h" 

int loan_id_counter = 1;

Loan* loan_create(int id, Book *book, User *user, Date dataLoan, Date dataExpected) {
    Loan *newLoan = (Loan*)malloc(sizeof(Loan));
    if (!newLoan) return NULL;
    
    newLoan->id = id;
    newLoan->book = book;
    newLoan->leitor = user;
    newLoan->DataLoan = dataLoan;
    newLoan->DateExpected = dataExpected;
    newLoan->DateReturn = (Date){0, 0, 0}; 
    newLoan->status = LOAN_ACTIVE;
    newLoan->nextGlobal = NULL;
    newLoan->nextUser = NULL;
    
    return newLoan;
}

Loan* loan_find_by_user_and_book(LoanList *list, int userId, int bookId) {
    if (!list) return NULL;
    
    Loan *current = list->head;
    while (current) {
        if (current->leitor->id == userId && 
            current->book->id == bookId && 
            current->status == LOAN_ACTIVE) {
            return current;
        }
        current = current->nextGlobal;
    }
    return NULL;
}

void loan_add_to_global(LoanList *list, Loan *loan) {
    if (!list || !loan) return;
    
   
    if (!list->head) {
        list->head = loan;
        list->tail = loan;
    } else {
        list->tail->nextGlobal = loan;
        list->tail = loan;
    }
    list->quantity++;
}

void loan_add_to_user(User *user, Loan *loan) {
    if (!user || !loan) return;
    
    loan->nextUser = NULL;
    
    if (user->myLoans.head == NULL) {
        user->myLoans.head = loan;
        user->myLoans.tail = loan;
    } else {
        user->myLoans.tail->nextUser = loan;
        user->myLoans.tail = loan; 
    }
    
    user->myLoans.quantity++;
    user->activeLoans++;
}

void loan_print_user_loans(User *user) {
    if (!user) {
        printf("Usuário inválido.\n");
        return;
    }
    
    if (user->myLoans.head == NULL) {
        printf("Nenhum empréstimo ativo.\n");
        return;
    }
    
    printf("\n EMPRÉSTIMOS ATIVOS DE %s:\n", user->name);
    
    Loan *current = user->myLoans.head;
    int count = 1;
    
    while (current) {
        if (current->status == LOAN_ACTIVE) {
            printf("%d. Livro: %s (ID: %d)\n", count, current->book->title, current->book->id);
            printf("   Data Empréstimo: %02d/%02d/%04d\n", 
                   current->DataLoan.day,
                   current->DataLoan.month,
                   current->DataLoan.year);
            printf(" Data Prevista: %02d/%02d/%04d\n",
                   current->DateExpected.day,
                   current->DateExpected.month,
                   current->DateExpected.year);
            
            Date today = date_today();
            if (date_compare(today, current->DateExpected) > 0) {
                printf("ATRASADO!\n");
            }
            count++;
        }
        current = current->nextUser;
    }
}

void loan_print_all_active(LoanList *globalList) {
    if (!globalList || globalList->quantity == 0) {
        printf("Nenhum empréstimo ativo.\n");
        return;
    }
    
    printf("\n TODOS OS EMPRÉSTIMOS ATIVOS:\n");
    
    Loan *current = globalList->head;
    int count = 1;
    
    while (current) {
        printf("%d. ID: %d | Livro: %s | Usuário: %s\n", 
               count, 
               current->id,
               current->book->title,
               current->leitor->name);
        printf("   Data: %02d/%02d/%04d → %02d/%02d/%04d\n",
               current->DataLoan.day,
               current->DataLoan.month,
               current->DataLoan.year,
               current->DateExpected.day,
               current->DateExpected.month,
               current->DateExpected.year);
        count++;
        current = current->nextGlobal;
    }
}

void loan_add_to_history(HistoryList *history, Loan *loan) {
    if (!history || !loan) return;
    
    Loan *historyLoan = (Loan*)malloc(sizeof(Loan));
    if (!historyLoan) {
        printf("Erro de memória ao adicionar ao histórico.\n");
        return;
    }
    
    memcpy(historyLoan, loan, sizeof(Loan));
    
    historyLoan->nextGlobal = NULL;
    historyLoan->nextUser = NULL;
    
    if (!history->head) {
        history->head = historyLoan;
        history->tail = historyLoan;
    } else {
        history->tail->nextGlobal = historyLoan;
        history->tail = historyLoan;
    }
    history->quantity++;
}

void process_reservations_after_return(LoanList *globalList, HistoryList *history, Book *book) {
    if (!book) return;
    
    if (!book->reservations || book->reservations->size == 0) {
        return;
    }
    
    User *nextUser = reserve_pop(book);
    
    if (nextUser) {
        Date hoje = date_today();
        Date dataPrevista = date_add_days(hoje, 15);
        
        Loan *newLoan = loan_create(
            loan_id_counter++,
            book,
            nextUser,
            hoje,
            dataPrevista
        );
        
        if (newLoan) {
            loan_add_to_global(globalList, newLoan);
            loan_add_to_user(nextUser, newLoan);
            book->availableQuantity--;
            book->timesBorrowed++;  
            
            printf(" EMPRESTIMO AUTOMATICO REALIZADO!\n");
            printf(" Usuario: %s (ID: %d)\n", nextUser->name, nextUser->id);
            printf(" Livro: %s\n", book->title);
            printf(" Data Devolucao: %02d/%02d/%04d\n",
                   dataPrevista.day, dataPrevista.month, dataPrevista.year);
        }
        
        if (book->reservations && book->reservations->size > 0) {
            printf("\n   Ainda ha %d pessoa(s) na fila de espera.\n", 
                   book->reservations->size);
        }
    }
}

void loan_return_book(LoanList *globalList, HistoryList *history, Loan *loan) {
    if (!loan || !globalList || !history) {
        printf("Erro: Parametros invalidos para devolucao.\n");
        return;
    }
    
    loan->status = LOAN_RETURNED;
    loan->DateReturn = date_today();
    
    loan->book->availableQuantity++;
    
    Loan *prev = NULL;
    
    if (globalList->head == loan) {
        globalList->head = loan->nextGlobal;
    } else {
        prev = globalList->head;
        while (prev && prev->nextGlobal != loan) {
            prev = prev->nextGlobal;
        }
        if (prev) {
            prev->nextGlobal = loan->nextGlobal;
        }
    }
    
    if (globalList->tail == loan) {
        globalList->tail = prev;
    }
    globalList->quantity--;
    
    User *user = loan->leitor;
    Loan *prevUser = NULL;
    
    if (user->myLoans.head == loan) {
        user->myLoans.head = loan->nextUser;
    } else {
        prevUser = user->myLoans.head;
        while (prevUser && prevUser->nextUser != loan) {
            prevUser = prevUser->nextUser;
        }
        if (prevUser) {
            prevUser->nextUser = loan->nextUser;
        }
    }
    
    if (user->myLoans.tail == loan) {
        user->myLoans.tail = prevUser;
    }
    user->activeLoans--;
    
    loan->nextGlobal = NULL;
    loan->nextUser = NULL;
    
    loan_add_to_history(history, loan);
    
    process_reservations_after_return(globalList, history, loan->book);
}


void process_book_return(LoanList *globalList, HistoryList *history, 
                            AVLNode *bookRoot, int bookId, int userId) {
        
        printf("\n--- EFETUAR DEVOLUCAO ---\n");
        
        Loan *loan = loan_find_by_user_and_book(globalList, userId, bookId);
        
        if (!loan) {
            printf(" Emprestimo nao encontrado.\n");
            printf(" Verifique se o usuario tem este livro emprestado.\n");
            return;
        }
        
        if (loan->status != LOAN_ACTIVE) {
            printf("Este emprestimo ja foi devolvido.\n");
            return;
        }
        
        printf("\n Livro: %s (ID: %d)\n", loan->book->title, loan->book->id);
        printf(" Usuario: %s (ID: %d)\n", loan->leitor->name, loan->leitor->id);
        printf(" Data Emprestimo: %02d/%02d/%04d\n",
            loan->DataLoan.day,
            loan->DataLoan.month,
            loan->DataLoan.year);
        printf(" Data Prevista: %02d/%02d/%04d\n",
            loan->DateExpected.day,
            loan->DateExpected.month,
            loan->DateExpected.year);
        
        Date today = date_today();
        if (date_compare(today, loan->DateExpected) > 0) {
            printf("DEVOLUCAO ATRASADA!\n");
        }
        
        printf("\n Deseja confirmar a devolucao? (1-Sim / 0-Nao): ");
        int confirm;
        if (!safe_read_int(&confirm) || confirm != 1) {
            printf(" Devolucao cancelada.\n");
            return;
        }
        
        loan_return_book(globalList, history, loan);
        
        printf("\n DEVOLUCAO REALIZADA COM SUCESSO!\n");
        printf("   Livro: %s\n", loan->book->title);
        printf("   Usuario: %s\n", loan->leitor->name);
        printf("   Data Devolucao: %02d/%02d/%04d\n",
            loan->DateReturn.day,
            loan->DateReturn.month,
            loan->DateReturn.year);
    }

Loan* loan_find_by_book_id(LoanList *globalList, int bookId) {
        if (!globalList) return NULL;
        
        Loan *current = globalList->head;
        while (current) {
            if (current->book->id == bookId && current->status == LOAN_ACTIVE) {
                return current;
            }
            current = current->nextGlobal;
        }
        return NULL;
    }

Loan* loan_find_by_user_id(LoanList *globalList, int userId) {
        if (!globalList) return NULL;
        
        Loan *current = globalList->head;
        while (current) {
            if (current->leitor->id == userId && current->status == LOAN_ACTIVE) {
                return current;
            }
            current = current->nextGlobal;
        }
        return NULL;
    }

void loan_list_active_for_return(LoanList *globalList) {
        if (!globalList || globalList->quantity == 0) {
            printf("Nenhum emprestimo ativo para devolucao.\n");
            return;
        }
        
        printf("\n--- EMPRESTIMOS ATIVOS ---\n");
        printf("ID  | Livro                | Usuario              | Data Prevista\n");
        
        Loan *current = globalList->head;
        while (current) {
            if (current->status == LOAN_ACTIVE) {
                printf("%3d | %-20s | %-20s | %02d/%02d/%04d\n",
                    current->id,
                    current->book->title,
                    current->leitor->name,
                    current->DateExpected.day,
                    current->DateExpected.month,
                    current->DateExpected.year);
            }
            current = current->nextGlobal;
        }
    }


Loan* loan_find_by_id(LoanList *globalList, int id) {
        if (!globalList) return NULL;
        
        Loan *current = globalList->head;
        while (current) {
            if (current->id == id) {
                return current;
            }
            current = current->nextGlobal;
        }
        return NULL;
    }


void loan_free_global(LoanList *list) {
        if (!list) return;
        
        Loan *current = list->head;
        while (current) {
            Loan *next = current->nextGlobal;
            free(current);
            current = next;
        }
        list->head = NULL;
        list->tail = NULL;
        list->quantity = 0;
        free(list);
    }

void loan_free_history(HistoryList *history) {
        if (!history) return;
        
        Loan *current = history->head;
        while (current) {
            Loan *next = current->nextGlobal;
            free(current);
            current = next;
        }
        history->head = NULL;
        history->tail = NULL;
        history->quantity = 0;
        free(history);
    }

void load_loans_from_file(AVLNode *bookRoot, AVLNode *userRoot, 
                          LoanList **globalList, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Arquivo %s nao encontrado.\n", filename);
        return;
    }
    
    if (!*globalList) {
        *globalList = (LoanList*)malloc(sizeof(LoanList));
        (*globalList)->head = NULL;
        (*globalList)->tail = NULL;
        (*globalList)->quantity = 0;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        int loanId, bookId, userId, status;
        int d1, m1, y1, d2, m2, y2;
        
        sscanf(line, "%d,%d,%d,%d/%d/%d,%d/%d/%d,%d",
               &loanId, &bookId, &userId,
               &d1, &m1, &y1,
               &d2, &m2, &y2,
               &status);
        
        Book *book = book_find(bookRoot, bookId);
        User *user = user_find(userRoot, userId);
        
        if (!book || !user) {
            printf("Livro ou usuario nao encontrado para emprestimo %d\n", loanId);
            continue;
        }
        
        Loan *loan = (Loan*)malloc(sizeof(Loan));
        loan->id = loanId;
        loan->book = book;
        loan->leitor = user;
        loan->DataLoan = (Date){d1, m1, y1};
        loan->DateExpected = (Date){d2, m2, y2};
        loan->DateReturn = (Date){0, 0, 0};
        loan->status = (LoanStatus)status;
        loan->nextGlobal = NULL;
        loan->nextUser = NULL;
        
        if (loan->status == LOAN_ACTIVE) {
            loan_add_to_global(*globalList, loan);
            loan_add_to_user(user, loan);
            book->availableQuantity--;
        }
        
        if (loanId >= loan_id_counter) {
            loan_id_counter = loanId + 1;
        }
        
        loaded++;
    }
    
    fclose(file);
    printf("Carregados %d emprestimos do arquivo %s\n", loaded, filename);
}

void load_history_from_file(AVLNode *bookRoot, AVLNode *userRoot, 
                            HistoryList **history, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Arquivo %s nao encontrado.\n", filename);
        return;
    }
    
    if (!*history) {
        *history = (HistoryList*)malloc(sizeof(HistoryList));
        (*history)->head = NULL;
        (*history)->tail = NULL;
        (*history)->quantity = 0;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        int loanId, bookId, userId, status;
        int d1, m1, y1, d2, m2, y2, d3, m3, y3;
        
        sscanf(line, "%d,%d,%d,%d/%d/%d,%d/%d/%d,%d/%d/%d,%d",
               &loanId, &bookId, &userId,
               &d1, &m1, &y1,
               &d2, &m2, &y2,
               &d3, &m3, &y3,
               &status);
        
        Book *book = book_find(bookRoot, bookId);
        User *user = user_find(userRoot, userId);
        
        if (!book || !user) continue;
        
        Loan *loan = (Loan*)malloc(sizeof(Loan));
        loan->id = loanId;
        loan->book = book;
        loan->leitor = user;
        loan->DataLoan = (Date){d1, m1, y1};
        loan->DateExpected = (Date){d2, m2, y2};
        loan->DateReturn = (Date){d3, m3, y3};
        loan->status = (LoanStatus)status;
        loan->nextGlobal = NULL;
        loan->nextUser = NULL;
        
        loan_add_to_history(*history, loan);
        loaded++;
    }
    
    fclose(file);
    printf("Carregados %d historicos do arquivo %s\n", loaded, filename);
}

void save_loans_to_file(LoanList *list, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    Loan *current = list->head;
    while (current) {
        fprintf(file, "%d,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d,%d\n",
                current->id,
                current->book->id,
                current->leitor->id,
                current->DataLoan.day, current->DataLoan.month, current->DataLoan.year,
                current->DateExpected.day, current->DateExpected.month, current->DateExpected.year,
                current->status);
        current = current->nextGlobal;
    }
    
    fclose(file);
}

void save_history_to_file(HistoryList *history, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    Loan *current = history->head;
    while (current) {
        fprintf(file, "%d,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d,%02d/%02d/%04d,%d\n",
                current->id,
                current->book->id,
                current->leitor->id,
                current->DataLoan.day, current->DataLoan.month, current->DataLoan.year,
                current->DateExpected.day, current->DateExpected.month, current->DateExpected.year,
                current->DateReturn.day, current->DateReturn.month, current->DateReturn.year,
                current->status);
        current = current->nextGlobal;
    }
    
    fclose(file);
}


void loan_print_user_history(HistoryList *history, int userId) {
    if (!history || history->quantity == 0) {
        printf("Nenhum historico disponivel.\n");
        return;
    }
    
    Loan *current = history->head;
    int found = 0;
    
    printf("\n--- HISTORICO DO USUARIO (ID: %d) ---\n", userId);
    printf("ID  |  Livro              |  Data Emprestimo   |  Data Devolucao\n");
    printf("\n");
    
    while (current) {
        if (current->leitor->id == userId) {
            printf("%3d | %-20s | %02d/%02d/%04d | %02d/%02d/%04d\n",
                   current->id,
                   current->book->title,
                   current->DataLoan.day, current->DataLoan.month, current->DataLoan.year,
                   current->DateReturn.day, current->DateReturn.month, current->DateReturn.year);
            found++;
        }
        current = current->nextGlobal;
    }
    
    if (!found) {
        printf("Nenhum historico encontrado para este usuario.\n");
    }
}

void loan_print_all_history(HistoryList *history) {
    if (!history || history->quantity == 0) {
        printf("Nenhum historico disponivel.\n");
        return;
    }
    
    printf("\n--- HISTORICO COMPLETO ---\n");
    printf("ID  | Usuario              | Livro                | Data Emprestimo | Data Devolucao\n");
    printf("\n");
    
    Loan *current = history->head;
    while (current) {
        printf("%3d | %-20s | %-20s | %02d/%02d/%04d | %02d/%02d/%04d\n",
               current->id,
               current->leitor->name,
               current->book->title,
               current->DataLoan.day, current->DataLoan.month, current->DataLoan.year,
               current->DateReturn.day, current->DateReturn.month, current->DateReturn.year);
        current = current->nextGlobal;
    }
}