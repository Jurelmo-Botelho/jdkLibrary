#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl.h"
#include "book.h"
#include "user.h"
#include "loan.h"
#include "auth.h"
#include "files.h"
#include "ui.h"

static AVLTree *book_tree = NULL;
static AVLTree *user_tree = NULL;
static LoanList *loan_list = NULL;

static Session current_session;


// DECLARAÇÕES

void init_system(void);
void save_and_exit(void);

void handle_main_menu(void);
void handle_login(void);
void handle_create_account(void);

void handle_admin_menu(void);
void handle_student_menu(void);

void book_menu(void);
void user_menu(void);


// MAIN
int main(void)
{
    init_system();

    handle_main_menu();

    save_and_exit();

    return 0;
}

// INICIALIZAÇÃO

void init_system(void)
{

    user_load_next_id();

    book_tree = avl_create();
    user_tree = avl_create();
    loan_list = loan_list_create();

    if(book_tree == NULL || user_tree == NULL || loan_list == NULL)
    {
        printf("Erro ao iniciar estruturas\n");
        exit(1);
    }

    files_load_books(book_tree,"data/books.txt");

    files_load_users(user_tree,"data/users.txt");

    User *admin = user_find_by_username(user_tree,"admin");

    if(admin == NULL){
        admin = user_create_default_admin();

        if(admin)
            user_insert(user_tree,admin);
    }

    loan_list_load_from_file(loan_list, "data/loans.txt");

    current_session.authenticated = 0;
    current_session.user = NULL;

    printf("\nSistema iniciado!\n");

    ui_wait_enter();

}

// GUARDAR

void save_and_exit(void)
{
    files_save_books(book_tree, "data/books.txt");

    files_save_users(user_tree, "data/users.txt");

    loan_list_save_to_file(loan_list, "data/loans.txt");

    avl_destroy(book_tree,NULL);

    avl_destroy(user_tree, user_destroy_callback);

    loan_list_destroy(loan_list);
}

// MENU PRINCIPAL

void handle_main_menu(void)
{
    int option;

    do{
        ui_show_main_menu();

        option = ui_get_int("");

        switch(option)
        {

            case 1:
                handle_login();
                break;

            case 2:
                handle_create_account();
                break;

            case 0:
                break;

            default:
                ui_show_error("Opcao invalida");
        }

    }while(option != 0);

}

// LOGIN

void handle_login(void)
{
    char username[MAX_USERNAME];

    ui_get_string("Username: ", username, MAX_USERNAME);

    char *password = ui_get_login_password();

    current_session = auth_login(user_tree, username, password);

    if(current_session.authenticated)
    {
        ui_show_login_success(current_session.user->name);

        if(current_session.user->role == ROLE_ADMIN)
            handle_admin_menu();
        else
            handle_student_menu();
    } else {
        ui_show_login_failed();
    }

    ui_wait_enter();

}

// CRIAR CONTA

void handle_create_account(void)
{
    char username[MAX_USERNAME];
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char password[100];

    ui_get_string("Username: ", username, MAX_USERNAME);

    normalize_username(username);

    if(user_username_exists(user_tree,username))
    {
        ui_show_error("Username existente");

        ui_wait_enter();
        return;
    }

    ui_get_string("Nome: ", name, MAX_NAME);

    ui_get_string("Telefone: ", phone, MAX_PHONE);

    ui_get_string("Password: ", password, 100);

    User *user = user_create(0, username, name, phone, ROLE_STUDENT, password);

    if(user){
        user_insert(user_tree,user);

        ui_show_success("Conta criada");
    }

    ui_wait_enter();
}

// MENU ADMIN

void handle_admin_menu(void)
{
    int op;

    do{
        ui_show_admin_menu();

        op = ui_get_int("");

        switch(op)
        {
            case 1:
                book_menu();
                break;

            case 2:
                user_menu();
                break;

            case 3:
                ui_show_message("Relatorios Em desenvolvimento...");
                ui_wait_enter();
                break;

            case 4:
                auth_logout(&current_session);
                break;
        }

    }while(op != 4);

}

// MENU LIVROS

void book_menu(void)
{
    int op;

    do{

        printf("\n1 - Listar livros");
        printf("\n2 - Procurar livro");
        printf("\n3 - Adicionar livro");
        printf("\n4 - Remover livro");
        printf("\n0 - Voltar\n");

        op = ui_get_int("Opcao: ");

        if(op==1)
        {
            avl_inorder(book_tree, (void*)book_print);

            ui_wait_enter();

        }else if(op==2){

            int code = ui_get_int("Codigo: ");

            Book *b = avl_search(book_tree, code);

            book_print(b);

            ui_wait_enter();
        }

        else if(op==3)
        {
            int code = ui_get_int("Codigo: ");

            char title[MAX_TITLE];

            ui_get_string("Titulo:", title, MAX_TITLE);

            Book *b = book_create(code, title, "", "", 2025, "", 1);

            avl_insert(book_tree, code, b);

        } else if(op==4){

            int code = ui_get_int("Codigo: ");

            Book *b = avl_search(book_tree, code);

            if(b){
                avl_remove(book_tree, code);

                free(b);
            }
        }

    }while(op!=0);

}

// MENU UTILIZADORES

void user_menu(void)
{
    int op;

    do {
        printf("\n===== GESTAO DE UTILIZADORES =====\n");
        printf("1 - Listar utilizadores\n");
        printf("2 - Criar utilizador\n");
        printf("3 - Remover utilizador\n");
        printf("4 - Atualizar username\n");
        printf("5 - Atualizar password\n");
        printf("0 - Voltar\n");
        printf("Opcao: ");

        op = ui_get_int("");

        if (op == 1)
        {
            user_list_all(user_tree);
            ui_wait_enter();
        }

        else if (op == 2)
        {
            char username[MAX_USERNAME];
            char name[MAX_NAME];
            char phone[MAX_PHONE];
            char password[100];

            ui_get_string("Username: ", username, MAX_USERNAME);

            normalize_username(username);

            if (user_username_exists(user_tree, username))
            {
                ui_show_error("Username ja existe!");
                ui_wait_enter();
                continue;
            }

            ui_get_string("Nome: ", name, MAX_NAME);
            ui_get_string("Telefone: ", phone, MAX_PHONE);
            ui_get_string("Password: ", password, 100);

            User *user = user_create(
                0,
                username,
                name,
                phone,
                ROLE_STUDENT,
                password
            );

            if (user && user_insert(user_tree, user))
                ui_show_success("Utilizador criado!");
            else
                ui_show_error("Erro ao criar utilizador!");

            ui_wait_enter();
        }

        else if (op == 3)
        {
            int id = ui_get_int("ID: ");

            if (user_delete(user_tree, id))
                ui_show_success("Utilizador removido!");
            else
                ui_show_error("Utilizador nao encontrado!");

            ui_wait_enter();
        }

        else if (op == 4)
        {
            int id = ui_get_int("ID: ");

            char new_username[MAX_USERNAME];
            ui_get_string("Novo username: ", new_username, MAX_USERNAME);

            normalize_username(new_username);

            if (user_update_username(user_tree, id, new_username))
                ui_show_success("Username atualizado!");
            else
                ui_show_error("Erro ao atualizar username!");

            ui_wait_enter();
        }

        else if (op == 5)
        {
            int id = ui_get_int("ID: ");

            char pass[100];
            ui_get_string("Nova password: ", pass, 100);

            if (user_update_password(user_tree, id, pass))
                ui_show_success("Password atualizada!");
            else
                ui_show_error("Erro ao atualizar password!");

            ui_wait_enter();
        }

    } while (op != 0);
}

// MENU ESTUDANTE

void handle_student_menu(void)
{
    int op;

    do{

        ui_show_student_menu();

        op = ui_get_int("");

        if(op==4){
            auth_logout(&current_session);
        }

    }while(op!=4);
}