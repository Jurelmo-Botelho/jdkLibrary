#ifndef USER_H
#define USER_H

#include "types.h"

typedef struct {
    int  number;                      
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    Role role;                 
    char password_hash[MAX_PASSWORD_HASH]; 
    int  active_loans;               
} User;

#endif 
