#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"

static void clear_input_buffer(void){

    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

int safe_read_string(char *buffer, int size)
{
    if (buffer == NULL || size <= 1)
        return 0;


    if (fgets(buffer, size, stdin) == NULL)
        return 0;


    // Verifica se a entrada ultrapassou o tamanho do buffer
    if (strchr(buffer, '\n') == NULL)
    {
        clear_input_buffer();
    }
    else
    {
        // Remove o '\n'
        buffer[strcspn(buffer, "\n")] = '\0';
    }


    // Verifica se ficou vazio
    if (strlen(buffer) == 0)
    {
        printf("Erro: campo nao pode estar vazio.\n");
        return 0;
    }


    return 1;
}

int safe_read_int(int *value)
{
    char buffer[100];

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0;


    char extra;


    if (sscanf(buffer, "%d %c", value, &extra) != 1)
    {
        printf("Erro: valor inteiro invalido.\n");
        return 0;
    }

    return 1;
}

int confirm_action(const char *message)
{
    char answer[10];


    printf("%s (S/N): ", message);


    if(!safe_read_string(answer, sizeof(answer)))
        return 0;


    if(answer[0] == 'S' || answer[0] == 's')
        return 1;


    return 0;
}