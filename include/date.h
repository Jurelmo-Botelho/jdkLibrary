#ifndef DATE_H
#define DATE_H

#include "types.h"

typedef struct Date {
    int day;
    int month;
    int year;
} Date;

Date date_today(void);
Date date_add_days(Date d, int dias);
int date_compare(Date a, Date b);
int date_diff_days(Date a, Date b);
void date_to_string(Date d, char *buf);
int date_is_valid(Date d);

#endif