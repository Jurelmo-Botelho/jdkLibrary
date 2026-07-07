#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "book.h"
#include "avl.h"
#include "reserve.h"
#include "utils.h"
#include "auth.h"
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

    if (year < 1450 || year > 2100)
    {
        printf("Erro: ano invalido.\n");
        return NULL;
    }

    if (minAge < 0 || minAge > 120)
    {
        printf("Erro: idade minima invalida.\n");
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

    book->reservations = NULL; 

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
    {
        if (!validate_title(newTitle))
            return 0;

        strncpy(book->title, newTitle, MAX_TITLE - 1);
        book->title[MAX_TITLE - 1] = '\0';
    }

    if (newAuthor != NULL)
    {
        if (!validate_author(newAuthor))
            return 0;

        strncpy(book->author, newAuthor, MAX_AUTHOR - 1);
        book->author[MAX_AUTHOR - 1] = '\0';
    }

    if (newCategory != NULL)
    {
        if (!validate_category(newCategory))
            return 0;

        strncpy(book->category, newCategory, MAX_CATEGORY - 1);
        book->category[MAX_CATEGORY - 1] = '\0';
    }

    if (newMinAge >= 0)
    {
        if (newMinAge > 120)
        {
            printf("Erro: idade minima invalida.\n");
            return 0;
        }

        book->minAge = newMinAge;
    }

    printf("Livro atualizado com sucesso.\n");
    return 1;
}

Book *book_find_by_title(AVLNode *root, const char *title)
{
    if (root == NULL)
        return NULL;

    Book *book = (Book *)root->data;

    if (strcmp(book->title, title) == 0)
        return book;

    Book *found = book_find_by_title(root->left, title);

    if (found != NULL)
        return found;

    return book_find_by_title(root->right, title);
}

int book_find_by_author(AVLNode *root, const char *author)
{
    if (root == NULL)
        return 0;


    int count = 0;


    count += book_find_by_author(root->left, author);


    Book *book = (Book *)root->data;

    if (strcmp(book->author, author) == 0)
    {
        print_book(book);
        count++;
    }


    count += book_find_by_author(root->right, author);


    return count;
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

    int queueSize = (book->reservations != NULL) ? book->reservations->size : 0;

    printf("ID: %d | Título: %s | Autor: %s | Categoria: %s | Ano: %d | MinIdade: %d | Disponivel: %d/%d | Emprestimos: %d | EM espera: %d\n",
           book->id,
           book->title,
           book->author,
           book->category,
           book->year,
           book->minAge,
           book->availableQuantity,
           book->totalQuantity,
           book->timesBorrowed,
           queueSize);
}

void book_print_all(AVLNode *root)
{
    if (root == NULL)
        return;

    book_print_all(root->left);

    print_book(root->data);

    book_print_all(root->right);
}

int category_exists(AVLNode *root, const char *category)
{
    if (root == NULL)
        return 0;

    Book *book = (Book *)root->data;

    if (strcmp(book->category, category) == 0)
        return 1;

    return category_exists(root->left, category)
        || category_exists(root->right, category);
}

void book_print_by_category(AVLNode *root, const char *category)
{
    if (root == NULL)
        return;

    book_print_by_category(root->left, category);

    Book *book = (Book *)root->data;

    if (strcmp(book->category, category) == 0)
        print_book(book);

    book_print_by_category(root->right, category);
}

int category_already_exists(char categories[][MAX_CATEGORY], int size, const char *category){
    
    for (int i = 0; i < size; i++)
    {
        if (strcmp(categories[i], category) == 0)
            return 1;
    }

    return 0;
}

void count_categories_recursive(AVLNode *root,char categories[][MAX_CATEGORY], int *count){

    if (root == NULL)
        return;


    count_categories_recursive(root->left, categories, count);


    Book *book = (Book *)root->data;


    if (!category_already_exists(categories, *count, book->category))
    {
        strcpy(categories[*count], book->category);
        (*count)++;
    }

    count_categories_recursive(root->right, categories, count);
}

void free_book(void *data)
{
    if (data == NULL)
        return;

    Book *book = (Book *)data;

    if (book->reservations != NULL) {
        reserve_queue_destroy(book->reservations);
    }

    free(book);
}

int book_count_categories(AVLNode *root)
{
    char categories[100][MAX_CATEGORY];

    int count = 0;


    count_categories_recursive(root, categories, &count);


    return count;
}

void book_list_categories(AVLNode *root)
{
    char categories[100][MAX_CATEGORY];

    int count = 0;


    count_categories_recursive(root, categories, &count);


    if (count == 0)
    {
        printf("Nenhuma categoria encontrada.\n");
        return;
    }


    printf("\n===== CATEGORIAS =====\n");


    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, categories[i]);
    }
}

int book_get_category_by_index(AVLNode *root, int index, char *category)
{
    char categories[100][MAX_CATEGORY];

    int count = 0;


    count_categories_recursive(root, categories, &count);


    if (index < 1 || index > count)
        return 0;


    strcpy(category, categories[index - 1]);

    return 1;
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

    /*
    if (book->availableQuantity <= 0)
    {
        printf("Erro: livro indisponível.\n");
        return 0;
    } */

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

void book_print_unavailable(AVLNode *root) {
    if (!root) {
        printf("Nenhum livro cadastrado.\n");
        return;
    }
    
    if (root->left) {
        book_print_unavailable(root->left);
    }
    
    Book *book = (Book*)root->data;
    if (book->availableQuantity == 0) {
        printf("ID: %d | %s | %s | Fila: %d pessoas\n",
               book->id,
               book->title,
               book->author,
               book->reservations ? book->reservations->size : 0);
    }
    
    if (root->right) {
        book_print_unavailable(root->right);
    }
}

void load_books_from_file(AVLNode **bookRoot, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Arquivo %s nao encontrado.\n", filename);
        return;
    }
    
    char line[512];
    int loaded = 0;
    
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\0') continue;
        
        int id, year, minAge, totalQty, availableQty, timesBorrowed;
        char title[100], author[100], publisher[100], category[50];
        
        sscanf(line, "%d,%99[^,],%99[^,],%99[^,],%d,%49[^,],%d,%d,%d,%d",
               &id, title, author, publisher, &year, category, &minAge, 
               &totalQty, &availableQty, &timesBorrowed);
        
        Book *book = create_book(title, author, publisher, category, year, minAge, totalQty);
        if (book) {
            book->id = id;
            book->availableQuantity = availableQty;
            book->timesBorrowed = timesBorrowed;
            book->reservations = NULL;
            
            if (id >= book_id_counter) {
                book_id_counter = id + 1;
            }
            
            *bookRoot = book_insert(*bookRoot, book);
            loaded++;
        }
    }
    
    fclose(file);
    printf("Carregados %d livros do arquivo %s\n", loaded, filename);
}

void save_book_to_file(Book *book, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    fprintf(file, "%d,%s,%s,%s,%d,%s,%d,%d,%d,%d\n",
            book->id,
            book->title,
            book->author,
            book->publisher,
            book->year,
            book->category,
            book->minAge,
            book->totalQuantity,
            book->availableQuantity,
            book->timesBorrowed);
    
    fclose(file);
}

void save_books_to_file(AVLNode *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Erro ao abrir arquivo %s\n", filename);
        return;
    }
    
    save_books_recursive(root, file);
    fclose(file);
}

void save_books_recursive(AVLNode *node, FILE *file) {
    if (!node) return;
    
    save_books_recursive(node->left, file);
    
    Book *book = (Book*)node->data;
    fprintf(file, "%d,%s,%s,%s,%d,%s,%d,%d,%d,%d\n",
            book->id,
            book->title,
            book->author,
            book->publisher,
            book->year,
            book->category,
            book->minAge,
            book->totalQuantity,
            book->availableQuantity,
            book->timesBorrowed);
    
    save_books_recursive(node->right, file);
}
