#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

//criar fila 
FilaReservas* criarFila(int codigo_livro)
{
    FilaReservas *fila = (FilaReservas*) malloc(sizeof(FilaReservas));

    if (fila == NULL)
        return NULL;

    fila->codigo_livro = codigo_livro;
    fila->frente = NULL;
    fila->tras = NULL;
    fila->tamanho = 0;

    return fila;
}

//  fila vazia 
int filaVazia(FilaReservas *fila)
{
    if (fila == NULL)
        return 1;

    return (fila->frente == NULL);
}

// enfileirar 
void enfileirarReserva(FilaReservas *fila, int numero_utilizador, Date data)
{
    if (fila == NULL)
        return;

    NoReserva *novo = (NoReserva*) malloc(sizeof(NoReserva));

    if (novo == NULL)
        return;

    novo->numero_utilizador = numero_utilizador;
    novo->codigo_livro = fila->codigo_livro;
    novo->data_reserva = data;
    novo->proximo = NULL;

    if (fila->frente == NULL) {
        fila->frente = novo;
        fila->tras = novo;
    } else {
        fila->tras->proximo = novo;
        fila->tras = novo;
    }

    fila->tamanho++;
}

//  desenfileirar 
int desenfileirarReserva(FilaReservas *fila)
{
    if (fila == NULL || fila->frente == NULL)
        return 0;

    NoReserva *aux = fila->frente;

    fila->frente = fila->frente->proximo;

    if (fila->frente == NULL)
        fila->tras = NULL;

    free(aux);

    fila->tamanho--;

    return 1;
}

//  ver quem estaa frente da fila
NoReserva* frenteFila(FilaReservas *fila)
{
    if (fila == NULL)
        return NULL;

    return fila->frente;
}

// tamanho da fila
int tamanhoFila(FilaReservas *fila)
{
    if (fila == NULL)
        return 0;

    return fila->tamanho;
}

// destruir fila 
void destruirFila(FilaReservas *fila)
{
    if (fila == NULL)
        return;

    NoReserva *atual = fila->frente;

    while (atual != NULL) {
        NoReserva *prox = atual->proximo;
        free(atual);
        atual = prox;
    }

    free(fila);
}