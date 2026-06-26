#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"

// criar livro 
Livro* criarLivro(int codigo, const char *titulo, const char *autor,
                  const char *editora, int ano, const char *categoria,
                  int quantidade)
{
    Livro *l = (Livro*) malloc(sizeof(Livro));

    if (l == NULL)
        return NULL;

    l->codigo = codigo;

    strcpy(l->titulo, titulo);
    strcpy(l->autor, autor);
    strcpy(l->editora, editora);
    strcpy(l->categoria, categoria);
    l->ano = ano;
    l->quantidade_total = quantidade;
    l->quantidade_disponivel = quantidade;
    l->total_requisicoes = 0;

    return l;
}

// validar livro 
int validarLivro(Livro *l)
{
    if (l == NULL)
        return 0;

    if (l->codigo <= 0)
        return 0;

    if (strlen(l->titulo) == 0)
        return 0;

    if (strlen(l->autor) == 0)
        return 0;

    if (strlen(l->editora) == 0)
        return 0;

    if (l->ano < 1500 || l->ano > 2100)
        return 0;

    if (l->quantidade_total < 0)
        return 0;

    if (l->quantidade_disponivel < 0)
        return 0;

    if (l->quantidade_disponivel > l->quantidade_total)
        return 0;

    return 1;
}

// erificar disponibilidade 
int verificarDisponibilidade(Livro *l)
{
    if (l == NULL) {
        printf("Livro invalido!\n");
        return 0;
    }

    if (l->quantidade_disponivel > 0) {
        printf("Livro DISPONIVEL\n");
        return 1;
    } else {
        printf("Livro INDISPONIVEL\n");
        return 0;
    }
}

// atualizar livro 
void atualizarLivro(Livro *l, const char *titulo, const char *autor,
                    const char *editora, int ano, const char *categoria)
{
    if (l == NULL)
        return;

    if (strlen(titulo) > 0)
        strcpy(l->titulo, titulo);

    if (strlen(autor) > 0)
        strcpy(l->autor, autor);

    if (strlen(editora) > 0)
        strcpy(l->editora, editora);

    if (ano >= 1500 && ano <= 2100)
        l->ano = ano;

    if (strlen(categoria) > 0)
        strcpy(l->categoria, categoria);
}

//emprestar livro 
int emprestarLivro(Livro *l)
{
    if (l == NULL)
        return 0;

    if (l->quantidade_disponivel <= 0)
        return 0;

    l->quantidade_disponivel--;
    l->total_requisicoes++;

    return 1;
}

//devolver livro 
int devolverLivro(Livro *l)
{
    if (l == NULL)
        return 0;

    if (l->quantidade_disponivel >= l->quantidade_total)
        return 0;

    l->quantidade_disponivel++;

    return 1;
}

//imprimir livro 
void imprimirLivro(Livro *l)
{
    if (l == NULL)
        return;

    printf("\n_______ LIVRO ________\n");
    printf("Codigo: %d\n", l->codigo);
    printf("Titulo: %s\n", l->titulo);
    printf("Autor: %s\n", l->autor);
    printf("Editora: %s\n", l->editora);
    printf("Ano: %d\n", l->ano);
    printf("Categoria: %s\n", l->categoria);
    printf("Disponiveis: %d/%d\n",
           l->quantidade_disponivel,
           l->quantidade_total);
    printf("Total requisicoes: %d\n", l->total_requisicoes);
}

// buscar livro usando o codigo ou Título 
int buscarLivro(Livro *l, int codigo, const char *titulo)
{
    if (l == NULL)
        return 0;

    // busca por código
    if (codigo > 0 && l->codigo == codigo) {
        printf("\n===== LIVRO ENCONTRADO (POR ID) =====\n");
        imprimirLivro(l);
        return 1;
    }

    // buscar com o titulo
    if (titulo != NULL && strlen(titulo) > 0) {
        if (strstr(l->titulo, titulo) != NULL) {
            printf("\n===== LIVRO ENCONTRADO (POR TITULO) =====\n");
            imprimirLivro(l);
            return 1;
        }
    }

    return 0;
}