#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
typedef struct QueueNode {
    int  userNumber;   
    int  bookCode;     
    Date reservedOn;   
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;   
    QueueNode *back;   
    int  size;    
} WaitingQueue;

#endif
