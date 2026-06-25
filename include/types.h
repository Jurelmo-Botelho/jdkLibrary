#ifndef TYPES_H
#define TYPES_H

#define MAX_NAME          100
#define MAX_TITLE         150
#define MAX_AUTHOR        100
#define MAX_PUBLISHER     100
#define MAX_CATEGORY       50
#define MAX_PHONE          15
#define MAX_PASSWORD_HASH  20   
#define MAX_ACTIVE_LOANS    3
#define DEFAULT_LOAN_DAYS  15 

//Cursos disponíveis 
typedef enum {
    COURSE_EINF   = 0,   // Computer Engineering
    COURSE_CONT   = 1,   // Accounting
    COURSE_ECIVIL = 2,   // Civil Engineering
    COURSE_EQ     = 3,   // Chemical Engineering
    COURSE_EPI    = 4,   // Industrial Production Engineering
    COURSE_NONE   = 5    // for Admin and Librarian (no course)
} Course;

// Papel no sistema
typedef enum {
    ROLE_ADMIN     = 0,
    ROLE_LIBRARIAN = 1,
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

#endif 
