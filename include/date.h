#ifndef DATE_H
#define DATE_H

#include "types.h"

// Objectivo: pegar a hora actual do sistema
Date date_today(void);

// Adicionar dias a uma data (15 dias no caso do empréstimo)
Date date_add_days(Date d, int days);

// Comparar datas
int  date_compare(Date a, Date b);

// diferença nos dias
int  date_diff_days(Date a, Date b);

// formatar data para string
void date_to_string(Date d, char *buf);

// checar se a data é válida
int  date_is_valid(Date d);

#endif 
