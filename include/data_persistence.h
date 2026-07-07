#ifndef DATA_PERSISTENCE_H
#define DATA_PERSISTENCE_H

#include "avl.h"
#include "types.h"
#include "loan.h"

void load_all_data(AVLNode **userRoot, AVLNode **bookRoot, 
                   LoanList **globalLoans, HistoryList **history);
void save_all_data(AVLNode *userRoot, AVLNode *bookRoot, 
                   LoanList *globalLoans, HistoryList *history);
void save_all_data_auto(AVLNode *userRoot, AVLNode *bookRoot, 
                        LoanList *globalLoans, HistoryList *history);

#endif