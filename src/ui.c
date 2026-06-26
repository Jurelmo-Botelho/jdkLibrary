#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ui.h"

// Limpa tela 
void ui_clear_screen(void)
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void ui_show_login_header(void)
{
    ui_clear_screen();
    printf("\n=====================================\n");
    printf("          BIBLIOTECA         \n");
    printf("            LOGIN                    \n");
    printf("=====================================\n\n");
}

// Pede o ID do utilizador
int ui_get_login_id(void)
{
    int id;
    printf("ID: ");
    scanf("%d", &id);
    getchar(); 
    return id;
}

// Pede a senha
char* ui_get_login_password(void)
{
    static char password[100];
    printf("Password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
    return password;
}

// Mostra sucesso no login
void ui_show_login_success(const char *name)
{
    printf("\nBem-vindo, %s!\n", name);
}

// Mostra falha no login
void ui_show_login_failed(void)
{
    printf("\nID ou senha inválidos!\n");
}

// Mostra mensagem
void ui_show_message(const char *msg)
{
    printf("%s\n", msg);
}

void ui_show_error(const char *msg)
{
    printf(" %s\n", msg);
}

void ui_show_success(const char *msg)
{
    printf("%s\n", msg);
}

// Espera Enter
void ui_wait_enter(void)
{
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// Pede um inteiro
int ui_get_int(const char *prompt)
{
    int value;
    printf("%s", prompt);
    scanf("%d", &value);
    getchar();
    return value;
}

// Pede uma string
char* ui_get_string(const char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}


void ui_show_main_menu(void)
{
    printf("\n=====================================\n");
    printf(" SISTEMA DE GESTAO DE BIBLIOTECA\n");
    printf("=====================================\n");
    printf("1. Login\n");
    printf("2. Criar Conta\n");
    printf("0. Sair\n");
    printf("Opcao: ");
}

void ui_show_admin_menu(void)
{
    printf("\n=====================================\n");
    printf(" MENU ADMIN\n");
    printf("=====================================\n");
    printf("1. Gerir Livros\n");
    printf("2. Gerir Utilizadores\n");
    printf("3. Relatorios\n");
    printf("4. Logout\n");
    printf("Opcao: ");
}

void ui_show_student_menu(void)
{
    printf("\n=====================================\n");
    printf(" MENU USUÁRIO\n");
    printf("=====================================\n");
    printf("1. Procurar Livro\n");
    printf("2. Requisitar Livro\n");
    printf("3. Reservar Livro\n");
    printf("4. Logout\n");
    printf("Opcao: ");
}

void ui_show_book_menu(void)
{
    printf("\n=====================================\n");
    printf(" GESTAO DE LIVROS\n");
    printf("=====================================\n");
    printf("1. Adicionar Livro\n");
    printf("2. Procurar Livro\n");
    printf("3. Atualizar Livro\n");
    printf("4. Remover Livro\n");
    printf("5. Listar Livros\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
}

void ui_show_report_menu(void)
{
    printf("\n=====================================\n");
    printf(" RELATORIOS\n");
    printf("=====================================\n");
    printf("1. Livros mais requisitados\n");
    printf("2. Emprestimos ativos\n");
    printf("3. Livros disponiveis\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
}