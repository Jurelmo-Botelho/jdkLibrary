#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loan.h"


//  CRIAÇÃO E DESTRUIÇÃO
LoanList* loan_list_create(void)
{
    LoanList *list = (LoanList*)malloc(sizeof(LoanList));
    if (list == NULL) return NULL;

    list->head = NULL;
    list->total = 0;
    list->next_id = 1;

    return list;
}

void loan_list_destroy(LoanList *list)
{
    if (list == NULL) return;

    LoanNode *current = list->head;
    while (current != NULL) {
        LoanNode *next = current->next;
        free(current);
        current = next;
    }

    free(list);
}


//  OPERAÇÕES BÁSICAS
int loan_list_add(LoanList *list, Loan *loan)
{
    if (list == NULL || loan == NULL) return 0;

    loan->id = list->next_id++;

    LoanNode *node = (LoanNode*)malloc(sizeof(LoanNode));
    if (node == NULL) return 0;

    node->loan = *loan;
    node->next = NULL;

    if (list->head == NULL) {
        list->head = node;
    } else {
        LoanNode *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }

    list->total++;
    return 1;
}

Loan* loan_list_find_by_id(LoanList *list, int id)
{
    if (list == NULL) return NULL;

    LoanNode *current = list->head;
    while (current != NULL) {
        if (current->loan.id == id) {
            return &current->loan;
        }
        current = current->next;
    }

    return NULL;
}


//  FUNÇÕES PARA FICHEIROS

Loan* loan_from_string(const char *line)
{
    if (line == NULL) return NULL;

    Loan *loan = (Loan*)malloc(sizeof(Loan));
    if (loan == NULL) return NULL;

    int result = sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                        &loan->id,
                        &loan->book_code,
                        &loan->user_number,
                        &loan->loan_date.day,
                        &loan->loan_date.month,
                        &loan->loan_date.year,
                        &loan->due_date.day,
                        &loan->due_date.month,
                        &loan->due_date.year,
                        &loan->return_date.day,
                        &loan->return_date.month,
                        &loan->return_date.year,
                        (int*)&loan->status);

    if (result != 13) {
        free(loan);
        return NULL;
    }

    return loan;
}

void loan_to_string(Loan *loan, char *buffer, int buffer_size)
{
    if (loan == NULL || buffer == NULL) return;

    snprintf(buffer, buffer_size,
             "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
             loan->id,
             loan->book_code,
             loan->user_number,
             loan->loan_date.day,
             loan->loan_date.month,
             loan->loan_date.year,
             loan->due_date.day,
             loan->due_date.month,
             loan->due_date.year,
             loan->return_date.day,
             loan->return_date.month,
             loan->return_date.year,
             loan->status);
}

int loan_list_save_to_file(LoanList *list, const char *filename)
{
    if (list == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "w");
    if (file == NULL) return 0;

    LoanNode *current = list->head;
    int count = 0;

    while (current != NULL) {
        char buffer[512];
        loan_to_string(&current->loan, buffer, sizeof(buffer));
        fprintf(file, "%s\n", buffer);
        current = current->next;
        count++;
    }

    fclose(file);
    return count;
}

int loan_list_load_from_file(LoanList *list, const char *filename)
{
    if (list == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    char line[512];
    int count = 0;
    int max_id = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        Loan *loan = loan_from_string(line);
        if (loan == NULL) continue;

        LoanNode *node = (LoanNode*)malloc(sizeof(LoanNode));
        if (node == NULL) {
            free(loan);
            continue;
        }

        node->loan = *loan;
        node->next = NULL;
        free(loan);

        if (list->head == NULL) {
            list->head = node;
        } else {
            LoanNode *current = list->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = node;
        }

        if (node->loan.id > max_id) {
            max_id = node->loan.id;
        }

        count++;
    }

    fclose(file);

    list->next_id = max_id + 1;
    list->total = count;

    return count;
}