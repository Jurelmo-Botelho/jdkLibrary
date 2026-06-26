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


//  VARIÁVEIS GLOBAIS DAS ESTRUTURAS 
static AVLTree *book_tree = NULL;
static AVLTree *user_tree = NULL;
static LoanList *loan_list = NULL;
static Session current_session;

//  DECLARAÇÕES
void init_system(void);
void save_and_exit(void);
void handle_main_menu(void);
void handle_login(void);
void handle_create_account(void);
void handle_admin_menu(void);
void handle_student_menu(void);

//  MAIN
int main(void)
{
    init_system();
    handle_main_menu();
    save_and_exit();
    return 0;
}


//  INICIALIZAÇÃO
void init_system(void)
{
    printf("=====================================\n");
    printf(" INICIALIZANDO SISTEMA...\n");
    printf("=====================================\n\n");

    user_load_next_id();

    // Cria árvores e lista de empréstimos
    book_tree = avl_create();
    user_tree = avl_create();
    loan_list = loan_list_create();

    if (book_tree == NULL || user_tree == NULL || loan_list == NULL) {
        printf("ERRO: Não foi possível criar estruturas.\n");
        exit(1);
    }

    // Carrega livros do ficheiro
    int book_count = files_load_books(book_tree, "data/books.txt");

    // Carrega utilizadores do ficheiro
    int user_count = files_load_users(user_tree, "data/users.txt");

    User *admin = user_create_default_admin();
    if (admin != NULL) {
        user_insert(user_tree, admin);
    }
   
    printf("Carregados %d utilizadores do ficheiro\n", user_count);

    // Carrega empréstimos 
    int loan_count = loan_list_load_from_file(loan_list, "data/loans.txt");
    if (loan_count > 0) {
        printf("Carregados %d empréstimos\n", loan_count);
    }

    current_session.authenticated = 0;
    current_session.user = NULL;

    printf("\n Sistema inicializado com sucesso!\n");
    printf("   Livros: %d | Utilizadores: %d | Empréstimos: %d\n",
           book_tree->total, user_tree->total, loan_list->total);
    
    ui_wait_enter();
}


//GUARDAR E SAIR 
void save_and_exit(void)
{
    printf("\n=====================================\n");
    printf("   A GUARDAR DADOS...\n");
    printf("=====================================\n");

    user_save_next_id(); 

    files_save_books(book_tree, "data/books.txt");
    files_save_users(user_tree, "data/users.txt");
    loan_list_save_to_file(loan_list, "data/loans.txt");

    // Liberta memória
    avl_destroy(book_tree, NULL);
    avl_destroy(user_tree, NULL);
    loan_list_destroy(loan_list);

    printf("Dados guardados. Sistema encerrado.\n");
}

//  MENU PRINCIPAL
void handle_main_menu(void)
{
    int option;

    do {
        ui_show_main_menu();
        option = ui_get_int("");

        switch (option) {
            case 1:
                handle_login();
                break;
            case 2:
                handle_create_account();
                break;
            case 0:
                printf("A sair...\n");
                break;
            default:
                ui_show_error("Opção inválida!");
        }
    } while (option != 0 && !current_session.authenticated);
}


//  LOGIN
void handle_login(void)
{
    ui_show_login_header();

    int id = ui_get_login_id();
    char *password = ui_get_login_password();

    current_session = auth_login(user_tree, id, password);

    if (current_session.authenticated) {
        ui_show_login_success(current_session.user->name);
        ui_wait_enter();

        if (current_session.user->role == ROLE_ADMIN) {
            handle_admin_menu();
        } else {
            handle_student_menu();
        }
    } else {
        ui_show_login_failed();
        ui_wait_enter();
    }
}

//  CRIAR CONTA
void handle_create_account(void)
{
    ui_clear_screen();
    printf("\n=====================================\n");
    printf("       CRIAR CONTA                  \n");
    printf("=====================================\n\n");

    int id = 0;
    
    char name[MAX_NAME];
    ui_get_string("Nome: ", name, MAX_NAME);
    
    char phone[MAX_PHONE];
    ui_get_string("Telefone: ", phone, MAX_PHONE);
    
    char password[100];
    ui_get_string("Senha: ", password, sizeof(password));

    User *user = user_create(id, name, phone, ROLE_STUDENT, password);
    if (user == NULL) {
        ui_show_error("Erro ao criar utilizador!");
        ui_wait_enter();
        return;
    }

    if (user_insert(user_tree, user)) {
        ui_show_success("Conta criada com sucesso!");
        files_save_users(user_tree, "data/users.txt");
    } else {
        ui_show_error("Erro ao guardar utilizador!");
        user_destroy(user);
    }

    ui_wait_enter();
}

//MENU ADMIN
void handle_admin_menu(void)
{
    int option;

    do {
        ui_clear_screen();
        printf("\n=== ADMIN: %s ===\n", current_session.user->name);
        ui_show_admin_menu();
        option = ui_get_int("");

        switch (option) {
            case 1:
                ui_show_message("Gestão de Livros (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 2:
                ui_show_message("Gestão de Utilizadores (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 3:
                ui_show_message("Relatórios (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 4:
                auth_logout(&current_session);
                ui_show_message("Logout efetuado!");
                ui_wait_enter();
                break;
            default:
                ui_show_error("Opção inválida!");
        }
    } while (option != 4);
}

//  MENU ESTUDANTE (SÓ ESTRUTURA BÁSICA)
void handle_student_menu(void)
{
    int option;

    do {
        ui_clear_screen();
        printf("\n=== ESTUDANTE: %s ===\n", current_session.user->name);
        ui_show_student_menu();
        option = ui_get_int("");

        switch (option) {
            case 1:
                ui_show_message("Procurar Livro (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 2:
                ui_show_message("Requisitar Livro (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 3:
                ui_show_message("Reservar Livro (em desenvolvimento)");
                ui_wait_enter();
                break;
            case 4:
                auth_logout(&current_session);
                ui_show_message("Logout efetuado!");
                ui_wait_enter();
                break;
            default:
                ui_show_error("Opção inválida!");
        }
    } while (option != 4);
}