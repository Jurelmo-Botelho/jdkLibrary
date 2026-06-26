#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME          100
#define MAX_USERNAME      50
#define MAX_TITLE         150
#define MAX_AUTHOR        100
#define MAX_PUBLISHER     100
#define MAX_CATEGORY       50
#define MAX_PHONE          15
#define MAX_HASH_STRING    32 
#define MAX_ACTIVE_LOANS    3
#define DEFAULT_LOAN_DAYS  15 

// Papel no sistema
typedef enum {
    ROLE_ADMIN     = 1,
    ROLE_STUDENT   = 2
} Role;

// Estado do Empréstimo
typedef enum {
    LOAN_ACTIVE    = 0,
    LOAN_RETURNED  = 1,
    LOAN_OVERDUE   = 2
} LoanStatus;

// Data
typedef struct {
    int day;
    int month;
    int year;
} Date;

//Procura de Usernames
typedef struct {
    const char *username;
    int found;
} UsernameSearch;

#endif 
