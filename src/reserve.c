#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "reserve.h"
#include "user.h"
#include "book.h"
#include "loan.h"
#include "avl.h"
#include "date.h"

ReserveQueue* reserve_queue_create() {
    ReserveQueue *queue = (ReserveQueue*)malloc(sizeof(ReserveQueue));
    if (!queue) return NULL;
    
    queue->front = NULL;
    queue->tail = NULL;
    queue->size = 0;
    
    return queue;
}

void reserve_queue_destroy(ReserveQueue *queue) {
    if (!queue) return;
    
    ReserveNode *current = queue->front;
    while (current) {
        ReserveNode *next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

bool reserve_can_reserve(Book *book, User *user) {
    if (!book || !user) {
        printf("Livro ou usuario invalido.\n");
        return false;
    }
    
    if (book->availableQuantity > 0) {
        printf("O livro esta disponivel! Nao e necessario reservar.\n");
        return false;
    }
    
    if (reserve_is_in_queue(book, user)) {
        printf("Usuario ja esta na fila de reservas deste livro.\n");
        return false;
    }
    
    Loan *userLoan = user->myLoans.head;
    while (userLoan) {
        if (userLoan->book->id == book->id && userLoan->status == LOAN_ACTIVE) {
            printf("Usuario ja possui este livro emprestado.\n");
            return false;
        }
        userLoan = userLoan->nextUser;
    }
    
    return true;
}

bool reserve_add(Book *book, User *user) {
    if (!book || !user) {
        printf("Erro: Livro ou usuario invalido.\n");
        return false;
    }
    
    if (!reserve_can_reserve(book, user)) {
        return false;
    }
    
    if (!book->reservations) {
        book->reservations = reserve_queue_create();
        if (!book->reservations) {
            printf("Erro ao criar fila de reservas.\n");
            return false;
        }
    }
    
    ReserveNode *newNode = (ReserveNode*)malloc(sizeof(ReserveNode));
    if (!newNode) {
        printf("Erro de memoria ao criar reserva.\n");
        return false;
    }
    
    newNode->user = user;
    newNode->reserveDate = date_today();
    newNode->next = NULL;
    
    if (!book->reservations->front) {
        book->reservations->front = newNode;
        book->reservations->tail = newNode;
    } else {
        book->reservations->tail->next = newNode;
        book->reservations->tail = newNode;
    }
    book->reservations->size++;
    
    printf("Reserva realizada com sucesso!\n");
    printf("Usuario: %s (ID: %d)\n", user->name, user->id);
    printf("Posicao na fila: %d\n", book->reservations->size);
    
    return true;
}

bool reserve_is_in_queue(Book *book, User *user) {
    if (!book || !user || !book->reservations) {
        return false;
    }
    
    ReserveNode *current = book->reservations->front;
    while (current) {
        if (current->user->id == user->id) {
            return true;
        }
        current = current->next;
    }
    
    return false;
}

User* reserve_pop(Book *book) {
    if (!book || !book->reservations || book->reservations->size == 0) {
        return NULL;
    }
    
    ReserveQueue *queue = book->reservations;
    ReserveNode *first = queue->front;
    User *user = first->user;
    
    queue->front = first->next;
    
    if (!queue->front) {
        queue->tail = NULL;
    }
    
    queue->size--;
    
    free(first);
    
    if (queue->size == 0) {
        free(queue);
        book->reservations = NULL;
    }
    
    return user;
}

bool reserve_remove(Book *book, User *user) {
    if (!book || !user || !book->reservations || book->reservations->size == 0) {
        printf("Nao ha reservas para remover.\n");
        return false;
    }
    
    ReserveQueue *queue = book->reservations;
    ReserveNode *current = queue->front;
    ReserveNode *prev = NULL;
    
    while (current) {
        if (current->user->id == user->id) {
            if (prev) {
                prev->next = current->next;
            } else {
                queue->front = current->next;
            }
            
            if (current == queue->tail) {
                queue->tail = prev;
            }
            
            queue->size--;
            free(current);
            
            if (queue->size == 0) {
                free(queue);
                book->reservations = NULL;
            }
            
            printf("Reserva removida com sucesso.\n");
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    printf("Usuario nao encontrado na fila de reservas.\n");
    return false;
}

void reserve_print_queue(Book *book) {
    if (!book) {
        printf("Livro invalido.\n");
        return;
    }
    
    if (!book->reservations || book->reservations->size == 0) {
        printf("Nenhuma reserva para o livro '%s'.\n", book->title);
        return;
    }
    
    printf("\nFILA DE RESERVAS: %s (ID: %d)\n", book->title, book->id);
    printf("Total de reservas: %d\n", book->reservations->size);
    printf("----------------------------------------\n");
    
    ReserveNode *current = book->reservations->front;
    int position = 1;
    
    while (current) {
        printf("%d. %s (ID: %d) - Data: %02d/%02d/%04d\n",
               position,
               current->user->name,
               current->user->id,
               current->reserveDate.day,
               current->reserveDate.month,
               current->reserveDate.year);
        if (position == 1) {
            printf("   PROXIMO DA FILA!\n");
        }
        position++;
        current = current->next;
    }
}

void load_reservations_from_file(AVLNode *bookRoot, AVLNode *userRoot, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Arquivo %s nao encontrado.\n", filename);
        return;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        int bookId, userId;
        int d, m, y;
        
        sscanf(line, "%d,%d,%d/%d/%d", &bookId, &userId, &d, &m, &y);
        
        Book *book = book_find(bookRoot, bookId);
        User *user = user_find(userRoot, userId);
        
        if (!book || !user) {
            printf("Livro ou usuario nao encontrado para reserva\n");
            continue;
        }
        
        if (!book->reservations) {
            book->reservations = reserve_queue_create();
        }
        
        ReserveNode *newNode = (ReserveNode*)malloc(sizeof(ReserveNode));
        newNode->user = user;
        newNode->reserveDate = (Date){d, m, y};
        newNode->next = NULL;
        
        if (!book->reservations->front) {
            book->reservations->front = newNode;
            book->reservations->tail = newNode;
        } else {
            book->reservations->tail->next = newNode;
            book->reservations->tail = newNode;
        }
        book->reservations->size++;
        loaded++;
    }
    
    fclose(file);
    printf("Carregadas %d reservas do arquivo %s\n", loaded, filename);
}

void save_reservations_to_file(AVLNode *bookRoot, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    save_reservations_recursive(bookRoot, file);
    fclose(file);
}

void save_reservations_recursive(AVLNode *node, FILE *file) {
    if (!node) return;
    
    save_reservations_recursive(node->left, file);
    
    Book *book = (Book*)node->data;
    if (book->reservations && book->reservations->size > 0) {
        ReserveNode *current = book->reservations->front;
        while (current) {
            fprintf(file, "%d,%d,%02d/%02d/%04d\n",
                    book->id,
                    current->user->id,
                    current->reserveDate.day,
                    current->reserveDate.month,
                    current->reserveDate.year);
            current = current->next;
        }
    }
    
    save_reservations_recursive(node->right, file);
}