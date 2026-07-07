#include <stdio.h>
#include <stdlib.h>

int safe_read_int(int *value)
{
    int result = scanf("%d", value);

    if (result != 1)
    {
        printf("Erro: entrada invalida.\n");

        /* limpar buffer */
        while (getchar() != '\n');

        return 0;
    }

    return 1;
}