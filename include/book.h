#ifndef BOOK_H
#define BOOK_H

#include "types.h"

//Estrutura do livro
typedef struct {
    int codigo;                  
    char titulo[MAX_TITLE];
    char autor[MAX_AUTHOR];
    char editora[MAX_PUBLISHER];
    int ano;
    char categoria[MAX_CATEGORY];
    int quantidade_total;
    int quantidade_disponivel;
    int total_requisicoes;
} Livro;

// Todas as  Funções 

// criar livro
Livro* criarLivro(int codigo, const char *titulo, const char *autor,
                  const char *editora, int ano, const char *categoria,
                  int quantidade);

// validar dados do livro
int validarLivro(Livro *l);



// atualizar dados do livro
void atualizarLivro(Livro *l, const char *titulo, const char *autor,
                    const char *editora, int ano, const char *categoria);

// emprestar livro 
int emprestarLivro(Livro *l);

// devolver livro 
int devolverLivro(Livro *l);

// imprime dados do livro
void imprimirLivro(Livro *l);

// busca do livro
int buscarLivro(Livro *l, int codigo, const char *titulo);

#endif