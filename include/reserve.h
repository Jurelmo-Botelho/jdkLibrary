#ifndef RESERVE_H
#define RESERVE_H

#include "types.h"
#include "date.h"
#include <stdbool.h>
#include <stdio.h>

typedef struct ReserveNode {
    User *user;    
    Date reserveDate; 
    struct ReserveNode *next;  
} ReserveNode;

typedef struct ReserveQueue {
    ReserveNode *front; 
    ReserveNode *tail; 
    int size;     
} ReserveQueue;

ReserveQueue* reserve_queue_create();
void reserve_queue_destroy(ReserveQueue *queue);
bool reserve_add(Book *book, User *user);
bool reserve_is_in_queue(Book *book, User *user);
bool reserve_can_reserve(Book *book, User *user);
void reserve_print_queue(Book *book);
bool reserve_remove(Book *book, User *user);
User* reserve_pop(Book *book);

void load_reservations_from_file(AVLNode *bookRoot, AVLNode *userRoot, const char *filename);
void save_reservations_to_file(AVLNode *bookRoot, const char *filename);
void save_reservations_recursive(AVLNode *node, FILE *file);

#endif