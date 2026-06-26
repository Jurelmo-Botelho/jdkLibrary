#ifndef DATE_H
#define DATE_H

#include "types.h"

// Obter a data atual do sistema
Date date_today(void);

// Adicionar dias a uma data
Date date_add_days(Date d, int dias);

// Comparar datas
int date_compare(Date a, Date b);

// Diferença em dias
int date_diff_days(Date a, Date b);

// Converter data para string
void date_to_string(Date d, char *buf);

// Verificar se a data é válida
int date_is_valid(Date d);

#endif