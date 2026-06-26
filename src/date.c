#include <stdio.h>
#include <time.h>
#include <string.h>
#include "date.h"
#include "types.h"

// Obter Data atual do sistema
Date date_today(void)
{
    Date d;

    time_t t = time(NULL);
    struct tm *hoje = localtime(&t);

    d.day = hoje->tm_mday;
    d.month = hoje->tm_mon + 1;
    d.year = hoje->tm_year + 1900;

    return d;
}

// Adicionar dias 
Date date_add_days(Date d, int days)
{
    d.day += days;

    while (d.day > 30) {
        d.day -= 30;
        d.month++;

        if (d.month > 12) {
            d.month = 1;
            d.year++;
        }
    }

    return d;
}

// Comparar Dates
int date_compare(Date a, Date b)
{
    if (a.year != b.year)
        return (a.year > b.year) ? 1 : -1;

    if (a.month != b.month)
        return (a.month > b.month) ? 1 : -1;

    if (a.day != b.day)
        return (a.day > b.day) ? 1 : -1;

    return 0;
}

// Diferença simples em days
int date_diff_days(Date a, Date b)
{
    int totalA = a.year * 365 + a.month * 30 + a.day;
    int totalB = b.year * 365 + b.month * 30 + b.day;

    return totalA - totalB;
}

// Converter para string
void date_to_string(Date d, char *buf)
{
    sprintf(buf, "%02d/%02d/%04d",
            d.day,
            d.month,
            d.year);
}

// Validar Date
int date_is_valid(Date d)
{
    if (d.year < 1)
        return 0;

    if (d.month < 1 || d.month > 12)
        return 0;

    if (d.day < 1 || d.day > 31)
        return 0;

    return 1;
}