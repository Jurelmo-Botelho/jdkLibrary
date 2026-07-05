#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "book.h"
#include "validation.h"

int book_id_counter = 1;

Book *create_book(const char *title, const char *author, const char *publisher, const char *category, int year, int minAge, int totalQuantity){

    if (!validate_title(title))
        return NULL;

    if (!validate_author(author))
        return NULL;

    if (!validate_category(category))
        return NULL;

    if (totalQuantity <= 0)
    {
        printf("Erro: quantidade inválida.\n");
        return NULL;
    }

    Book *book = (Book *)malloc(sizeof(Book));

    if (book == NULL)
    {
        printf("Erro: falha na alocação de memória.\n");
        return NULL;
    }

    book->id = book_id_counter++;

    strncpy(book->title, title, MAX_TITLE);
    strncpy(book->author, author, MAX_AUTHOR);
    strncpy(book->publisher, publisher, MAX_PUBLISHER);
    strncpy(book->category, category, MAX_CATEGORY);

    book->year = year;
    book->minAge = minAge;

    book->totalQuantity = totalQuantity;
    book->availableQuantity = totalQuantity;

    book->timesBorrowed = 0;

    return book;
}

int update_book(AVLNode *root, int id, const char *newTitle, const char *newAuthor, const char *newCategory, int newMinAge){

    AVLNode *node = avl_search(root, id);

    if (node == NULL)
    {
        printf("Erro: livro nao encontrado.\n");
        return 0;
    }

    Book *book = (Book *)node->data;

    if (newTitle != NULL)
        strncpy(book->title, newTitle, MAX_TITLE);

    if (newAuthor != NULL)
        strncpy(book->author, newAuthor, MAX_AUTHOR);

    if (newCategory != NULL)
        strncpy(book->category, newCategory, MAX_CATEGORY);

    if (newMinAge >= 0)
        book->minAge = newMinAge;

    printf("Livro atualizado com sucesso.\n");
    return 1;
}

int delete_book(AVLNode **root, int id)
{
    if (root == NULL || *root == NULL)
    {
        printf("Erro: arvore invalida.\n");
        return 0;
    }

    *root = avl_remove(*root, id);

    printf("Livro removido com sucesso.\n");
    return 1;
}

void print_book(void *data)
{
    if (data == NULL)
    {
        printf("Erro: livro inválido.\n");
        return;
    }

    Book *book = (Book *)data;

    printf("ID: %d | Título: %s | Autor: %s | Categoria: %s | Ano: %d | MinIdade: %d | Disponivel: %d/%d | Emprestimos: %d\n",
           book->id,
           book->title,
           book->author,
           book->category,
           book->year,
           book->minAge,
           book->availableQuantity,
           book->totalQuantity,
           book->timesBorrowed);
}

void book_print_all(AVLNode *root)
{
    if (root == NULL)
        return;

    book_print_all(root->left);

    print_book(root->data);

    book_print_all(root->right);
}


void free_book(void *data)
{
    if (data == NULL)
        return;

    free(data);
}

AVLNode *book_insert(AVLNode *root, Book *book)
{
    if (book == NULL)
    {
        printf("Erro: livro inválido.\n");
        return root;
    }

    return avl_insert(root, book->id, book);
}

AVLNode *book_remove(AVLNode *root, int id)
{
    return avl_remove(root, id);
}

Book *book_find(AVLNode *root, int id)
{
    AVLNode *node = avl_search(root, id);

    if (node == NULL)
        return NULL;

    return (Book *)node->data;
}

int book_can_borrow(Book *book, int userAge)
{
    if (book == NULL)
    {
        printf("Erro: livro inválido.\n");
        return 0;
    }

    if (book->availableQuantity <= 0)
    {
        printf("Erro: livro indisponível.\n");
        return 0;
    }

    if (userAge < book->minAge)
    {
        printf("Erro: idade insuficiente para este livro.\n");
        return 0;
    }

    return 1;
}

int book_decrease_available(Book *book)
{
    if (book == NULL)
        return 0;

    if (book->availableQuantity <= 0)
    {
        printf("Erro: não há exemplares disponíveis.\n");
        return 0;
    }

    book->availableQuantity--;
    book->timesBorrowed++;

    return 1;
}

int book_increase_available(Book *book)
{
    if (book == NULL)
        return 0;

    if (book->availableQuantity >= book->totalQuantity)
    {
        printf("Erro: quantidade já está no máximo.\n");
        return 0;
    }

    book->availableQuantity++;

    return 1;
}