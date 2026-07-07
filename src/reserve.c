#include "reserve.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "book.h"
#include "user.h"
#include "date.h"

ReserveQueue *reserve_queue_create()
{
    ReserveQueue *queue = (ReserveQueue *)malloc(sizeof(ReserveQueue));
    if (!queue)
        return NULL;

    queue->front = NULL;
    queue->tail = NULL;
    queue->size = 0;

    return queue;
}

void reserve_queue_destroy(ReserveQueue *queue)
{
    if (!queue)
        return;

    ReserveNode *current = queue->front;
    while (current)
    {
        ReserveNode *next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}

bool reserve_is_in_queue(Book *book, User *user)
{
    if (!book || !user || !book->reservations)
    {
        return false;
    }

    ReserveNode *current = book->reservations->front;
    while (current)
    {
        if (current->user->id == user->id)
        {
            return true;
        }
        current = current->next;
    }

    return false;
}

bool reserve_can_reserve(Book *book, User *user)
{

    if (reserve_is_in_queue(book, user))
    {
        printf("Usuário já está na fila de reservas deste livro.\n");
        return false;
    }

    if (book->availableQuantity > 0)
    {
        printf("ℹ O livro está disponível! Não é necessário reservar.\n");
        return false;
    }

    return true;
}

bool reserve_add(Book *book, User *user)
{
    if (!book || !user)
    {
        printf("Erro: Livro ou usuário inválido.\n");
        return false;
    }

    if (!reserve_can_reserve(book, user))
    {
        return false;
    }

    if (!book->reservations)
    {
        book->reservations = reserve_queue_create();
        if (!book->reservations)
        {
            printf("Erro ao criar fila de reservas.\n");
            return false;
        }
    }

    ReserveNode *newNode = (ReserveNode *)malloc(sizeof(ReserveNode));
    if (!newNode)
    {
        printf(" Erro de memória ao criar reserva.\n");
        return false;
    }

    newNode->user = user;
    newNode->reserveDate = date_today();
    newNode->next = NULL;

    if (!book->reservations->front)
    {
        book->reservations->front = newNode;
        book->reservations->tail = newNode;
    }
    else
    {
        book->reservations->tail->next = newNode;
        book->reservations->tail = newNode;
    }
    book->reservations->size++;

    printf("\n RESERVA REALIZADA COM SUCESSO!\n");
    printf("   Livro: %s\n", book->title);
    printf("   Leitor: %s\n", user->name);
    printf("   Data da reserva: %02d/%02d/%04d\n",
           newNode->reserveDate.day,
           newNode->reserveDate.month,
           newNode->reserveDate.year);
    printf("   Posição na fila: %dº lugar\n", book->reservations->size);
    printf("   Quantidade de pessoas na frente: %d\n", book->reservations->size - 1);

    return true;
}

void reserve_print_queue(Book *book) {
    if (!book) {
        printf("Livro inválido.\n");
        return;
    }

    if (!book->reservations || book->reservations->size == 0) {
        printf("Nenhuma reserva para o livro '%s'.\n", book->title);
        return;
    }

    printf("\n FILA DE RESERVAS: %s (ID: %d)\n", book->title, book->id);
    printf("Total de reservas: %d\n", book->reservations->size);

    ReserveNode *current = book->reservations->front;
    int position = 1;

    while (current) {
        printf("%dº %s (ID: %d)\n",
               position,
               current->user->name, 
               current->user->id);
        printf("Data da reserva: %02d/%02d/%04d\n",
               current->reserveDate.day,
               current->reserveDate.month,
               current->reserveDate.year);

        if (position == 1) {
            printf("PRÓXIMO DA FILA!\n");
        }
        position++;
        current = current->next;
    }
}

bool reserve_remove(Book *book, User *user) {
    if (!book || !user || !book->reservations || book->reservations->size == 0) {
        printf(" Não há reservas para remover.\n");
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

    printf(" Usuário não encontrado na fila de reservas.\n");
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

    printf("Reserva processada: %s retirou o livro %s\n",
           user->name, book->title);

    free(first);

    if (queue->size == 0) {
        free(queue);
        book->reservations = NULL;
    }

    return user;
}


