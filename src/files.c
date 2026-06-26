#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "files.h"
#include "book.h"
#include "user.h"


//  FUNÇÕES AUXILIARES PARA PERCURSO
static void save_book_callback(void *data)
{
    if (data == NULL) return;
    
    Book *book = (Book*)data;
    char buffer[512];
    book_to_string(book, buffer, sizeof(buffer));
    
}

static void save_user_callback(void *data)
{
    if (data == NULL) return;
    
    User *user = (User*)data;
    printf("%d,%s,%s,%s,%d,%s,%d\n", 
           user->id, user->username, user->name, user->phone, 
           user->role, user->password_hash, user->active_loans);
}


//  CARREGAR DADOS
int files_load_books(AVLTree *book_tree, const char *filename)
{
    if (book_tree == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: Ficheiro %s não encontrado.\n", filename);
        return 0;
    }

    char line[512];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        Book *book = book_from_string(line);
        if (book == NULL) {
            printf("Erro: Linha inválida: %s\n", line);
            continue;
        }

        if (avl_insert(book_tree, book->code, book)) {
            count++;
        } else {
            printf("Erro: Não foi possível inserir livro %d\n", book->code);
            free(book);
        }
    }

    fclose(file);
    printf("Carregados %d livros\n", count);
    return count;
}

int files_load_users(AVLTree *user_tree, const char *filename)
{
    if (user_tree == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Aviso: Ficheiro %s não encontrado.\n", filename);
        return 0;
    }

    char line[512];
    int count = 0;
    int max_id = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        int id, role_int, active_loans;
        char username[MAX_USERNAME], name[MAX_NAME], phone[MAX_PHONE], password_hash[MAX_HASH_STRING];

        int result = sscanf(line, "%d,%[^,],%[^,],%[^,],%d,%[^,],%d",
                            &id, username, name, phone, &role_int, password_hash, &active_loans);

        if (result != 7) {
            printf("Erro: Linha inválida: %s\n", line);
            continue;
        }

        Role role = (Role)role_int;

        User *user = user_create_with_hash(id, username, name, phone, role, password_hash);
        if(user == NULL)
        {
            printf("Erro: Não foi possível criar user %d\n", id);
            continue;
        }


        if(user_find_by_username(user_tree, username) != NULL)
        {
            printf("Erro: Username duplicado\n");
            user_destroy(user);
            continue;
        }


        user->active_loans = active_loans;


        if(avl_insert(user_tree, user->id, user))
        {
            count++;
        } else {
            user_destroy(user);
            printf("Erro: Não foi possível inserir user %d\n", id);
        }

        if (id > max_id) {
            max_id = id;  
        }

    }

    fclose(file);

    user_update_next_id(max_id); 
    
    printf("Carregados %d utilizadores\n", count);
    return count;
}


//  GUARDAR DADOS
static void save_book_to_file(void *data, FILE *file)
{
    if (data == NULL || file == NULL) return;
    
    Book *book = (Book*)data;
    char buffer[512];
    book_to_string(book, buffer, sizeof(buffer));
    fprintf(file, "%s\n", buffer);
}

static void save_user_to_file(void *data, FILE *file)
{
    if (data == NULL || file == NULL) return;
    
    User *user = (User*)data;
    fprintf(file, "%d,%s,%s,%s,%d,%s,%d\n", 
            user->id, user->username, user->name, user->phone, 
            user->role, user->password_hash, user->active_loans);
}

static void save_book_wrapper(void *data)
{
    extern FILE *_book_save_file;
    save_book_to_file(data, _book_save_file);
}

static void save_user_wrapper(void *data)
{
    extern FILE *_user_save_file;
    save_user_to_file(data, _user_save_file);
}

FILE *_book_save_file = NULL;
FILE *_user_save_file = NULL;

int files_save_books(AVLTree *book_tree, const char *filename)
{
    if (book_tree == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "w");
    if (file == NULL) return 0;

    _book_save_file = file;
    avl_inorder(book_tree, save_book_wrapper);
    _book_save_file = NULL;

    fclose(file);
    return 1;
}

int files_save_users(AVLTree *user_tree, const char *filename)
{
    if (user_tree == NULL || filename == NULL) return 0;

    FILE *file = fopen(filename, "w");
    if (file == NULL) return 0;

    _user_save_file = file;
    avl_inorder(user_tree, save_user_wrapper);
    _user_save_file = NULL;

    fclose(file);
    return 1;
}


int files_count_lines(const char *filename)
{
    if (filename == NULL) return 0;

    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0;

    int count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') count++;
    }

    fclose(file);
    return count;
}