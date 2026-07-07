#include <stdio.h>
#include <locale.h>

#include "ui.h"

int main()
{
    setlocale(LC_ALL, "Portuguese"); 
    
    run_system();
    return 0;
}