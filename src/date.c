#include <stdio.h>
#include <time.h>
#include <string.h>
#include "date.h"
#include "types.h"

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

Date date_add_days(Date d, int dias)
{
    struct tm t = {0};
    t.tm_mday = d.day + dias; 
    t.tm_mon = d.month - 1;
    t.tm_year = d.year - 1900;
    t.tm_isdst = -1;

    mktime(&t);

    Date nova_data;
    nova_data.day = t.tm_mday;
    nova_data.month = t.tm_mon + 1;
    nova_data.year = t.tm_year + 1900;

    return nova_data;
}

int date_compare(Date a, Date b)
{
    if (a.year != b.year)   return (a.year > b.year) ? 1 : -1;
    if (a.month != b.month) return (a.month > b.month) ? 1 : -1;
    if (a.day != b.day)     return (a.day > b.day) ? 1 : -1;
    return 0;
}

int date_diff_days(Date a, Date b)
{
    struct tm ta = {0}, tb = {0};
    
    ta.tm_mday = a.day;    ta.tm_mon = a.month - 1;    ta.tm_year = a.year - 1900; ta.tm_isdst = -1;
    tb.tm_mday = b.day;    tb.tm_mon = b.month - 1;    tb.tm_year = b.year - 1900; tb.tm_isdst = -1;

    time_t time_a = mktime(&ta);
    time_t time_b = mktime(&tb);

    return (int)(difftime(time_a, time_b) / 86400);
}

void date_to_string(Date d, char *buf)
{
    sprintf(buf, "%02d/%02d/%04d", d.day, d.month, d.year);
}

int date_is_valid(Date d)
{
    if (d.year < 1 || d.month < 1 || d.month > 12 || d.day < 1)
        return 0;


    int dias_por_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0)) {
        dias_por_mes[2] = 29;
    }

    if (d.day > dias_por_mes[d.month])
        return 0;

    return 1;
}
