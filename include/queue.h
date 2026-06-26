#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

//  Nó da fila de reservas 
typedef struct NoReserva {
    int numero_utilizador;
    int codigo_livro;
    Date data_reserva;
    struct NoReserva *proximo;
} NoReserva;

// Fila por livro 
typedef struct {
    int codigo_livro;
    NoReserva *frente;
    NoReserva *tras;
    int tamanho;
} FilaReservas;



FilaReservas* criarFila(int codigo_livro);

int filaVazia(FilaReservas *fila);

void enfileirarReserva(FilaReservas *fila, int numero_utilizador, Date data);

int desenfileirarReserva(FilaReservas *fila);

NoReserva* frenteFila(FilaReservas *fila);

int tamanhoFila(FilaReservas *fila);

void destruirFila(FilaReservas *fila);

#endif