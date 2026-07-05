#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "auth.h"
#include "validation.h"
#include "book.h"
#include "user.h"

int safe_read_int(int *value)
{
    int result = scanf("%d", value);

    if (result != 1)
    {
        printf("Erro: entrada invalida.\n");

        /* limpar buffer */
        while (getchar() != '\n');

        return 0;
    }

    return 1;
}

void show_main_menu(void)
{
    printf("\n===== SISTEMA BIBLIOTECA =====\n");
    printf("1. Login\n");
    printf("2. Criar Conta\n");
    printf("0. Sair\n");
    printf("Opcao: ");
}

void show_user_menu(void)
{
    printf("\n===== MENU LEITOR =====\n");
    printf("1. Consultar livros\n");
    printf("2. Procurar livro\n");
    printf("3. Pedir emprestimo\n");
    printf("4. Meus emprestimos\n");
    printf("5. Minhas reservas\n");
    printf("6. Historico\n");
    printf("0. Logout\n");
    printf("Opcao: ");
}

void show_admin_menu(void)
{
    printf("\n===== MENU BIBLIOTECARIO =====\n");
    printf("1. Registar livro\n");
    printf("2. Atualizar livro\n");
    printf("3. Remover livro\n");
    printf("4. Consultar livros\n");
    printf("5. Registar utilizador\n");
    printf("6. Atualizar utilizador\n");
    printf("7. Remover utilizador\n");
    printf("8. Consultar utilizadores\n");
    printf("9. Emprestimo\n");
    printf("10. Devolucao\n");
    printf("11. Reservas\n");
    printf("12. Relatorios\n");
    printf("0. Logout\n");
    printf("Opcao: ");
}

void run_system(void)
{
    AVLNode *userRoot = NULL;
    AVLNode *bookRoot = NULL;

    create_default_admin(&userRoot);

    Session *session = create_session();

    int option;

    while (1)
    {
        //LOGIN / REGISTO

        if (!session->isLoggedIn)
        {
            show_main_menu();

            if (!safe_read_int(&option))
                continue;

            switch (option)
            {
                case 1:
                {
                    char username[MAX_USERNAME];
                    char password[MAX_PASSWORD];

                    printf("Username: ");
                    scanf("%s", username);

                    printf("Password: ");
                    scanf("%s", password);

                    login(session, userRoot, username, password);
                    break;
                }

                case 2:
                {
                    char username[MAX_USERNAME];
                    char password[MAX_PASSWORD];
                    char name[MAX_NAME];
                    int age;
                    char phone[MAX_PHONE];

                    printf("Username: ");
                    scanf("%s", username);

                    printf("Password: ");
                    scanf("%s", password);

                    printf("Nome: ");
                    scanf(" %[^\n]", name);

                    printf("Idade: ");
                    if (!safe_read_int(&age)) break;

                    printf("Telefone: ");
                    scanf("%s", phone);

                    register_user(&userRoot, username, password, name, age, phone, ROLE_STUDENT);
                    break;
                }

                case 0:
                    printf("A sair do sistema...\n");
                    destroy_session(session);
                    return;

                default:
                    printf("Opcao invalida.\n");
                    break;
            }
        }

        //ADMINISTRADOR LOGADO

        else
        {
            if (session->currentUser->role == ROLE_ADMIN)
            {
                show_admin_menu();

                if (!safe_read_int(&option))
                    continue;

                switch (option)
                {
                    //LIVROS

                    case 1: /* CRIAR LIVROS */
                    {
                        char title[MAX_TITLE];
                        char author[MAX_AUTHOR];
                        char publisher[MAX_PUBLISHER];
                        char category[MAX_CATEGORY];
                        int year, minAge, qty;

                        printf("Titulo: ");
                        scanf(" %[^\n]", title);

                        printf("Autor: ");
                        scanf(" %[^\n]", author);

                        printf("Editora: ");
                        scanf(" %[^\n]", publisher);

                        printf("Categoria: ");
                        scanf(" %[^\n]", category);

                        printf("Ano: ");
                        if (!safe_read_int(&year)) break;

                        printf("Idade minima: ");
                        if (!safe_read_int(&minAge)) break;

                        printf("Quantidade: ");
                        if (!safe_read_int(&qty)) break;

                        register_book(&bookRoot, title, author, publisher, category, year, minAge, qty);
                        break;
                    }

                    case 2: /* ATUALIZAR LIVROS */
                    {
                        int id;
                        printf("ID do livro: ");
                        if (!safe_read_int(&id)) break;

                        char title[MAX_TITLE];
                        char author[MAX_AUTHOR];
                        char category[MAX_CATEGORY];
                        int minAge;

                        printf("Novo titulo: ");
                        scanf(" %[^\n]", title);

                        printf("Novo autor: ");
                        scanf(" %[^\n]", author);

                        printf("Nova categoria: ");
                        scanf(" %[^\n]", category);

                        printf("Nova idade minima: ");
                        if (!safe_read_int(&minAge)) break;

                        update_book(bookRoot, id, title, author, category, minAge);
                        break;
                    }

                    case 3: /* DELETAR LIVRO */
                    {
                        int id;
                        printf("ID do livro: ");
                        if (!safe_read_int(&id)) break;

                        delete_book(&bookRoot, id);
                        break;
                    }

                    case 4: /* LISTAR LIVROS */
                        printf("\n--- LIVROS ---\n");
                        book_print_all(bookRoot);
                        break;

                    //USERS

                    case 5: /* CRIAR USER */
                    {
                        char username[MAX_USERNAME];
                        char password[MAX_PASSWORD];
                        char name[MAX_NAME];
                        int age;
                        char phone[MAX_PHONE];

                        printf("Username: ");
                        scanf("%s", username);

                        printf("Password: ");
                        scanf("%s", password);

                        printf("Nome: ");
                        scanf(" %[^\n]", name);

                        printf("Idade: ");
                        if (!safe_read_int(&age)) break;

                        printf("Telefone: ");
                        scanf("%s", phone);

                        register_user(&userRoot, username, password, name, age, phone, ROLE_STUDENT);
                        break;
                    }

                    case 6: /* ATUALIZAR USER */
                    {
                        int id;
                        printf("ID do utilizador: ");
                        if (!safe_read_int(&id)) break;

                        char name[MAX_NAME];
                        int age;
                        char phone[MAX_PHONE];

                        printf("Novo nome: ");
                        scanf(" %[^\n]", name);

                        printf("Nova idade: ");
                        if (!safe_read_int(&age)) break;

                        printf("Novo telefone: ");
                        scanf("%s", phone);

                        update_user(userRoot, id, name, age, phone);
                        break;
                    }

                    case 7: /* DELETAR USER */
                    {
                        int id;
                        printf("ID do utilizador: ");
                        if (!safe_read_int(&id)) break;

                        delete_user(&userRoot, id);
                        break;
                    }

                    case 8: /* LISTAR USERS */
                        printf("\n--- UTILIZADORES ---\n");
                        user_print_all(userRoot);
                        break;

                    //EMPRÉSTIMOS

                    case 9:
                        printf("Emprestimo ainda nao implementado.\n");
                        break;

                    case 10:
                        printf("Devolucao ainda nao implementada.\n");
                        break;

                    case 11:
                        printf("Reservas ainda nao implementadas.\n");
                        break;

                    case 12:
                        printf("Relatorios ainda nao implementados.\n");
                        break;

                    case 0:
                        logout(session);
                        break;

                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
            }

            //LEITOR/USER LOGADO
            
            else
            {
                show_user_menu();

                if (!safe_read_int(&option))
                    continue;

                switch (option)
                {
                    case 1:
                        printf("\n--- LIVROS ---\n");
                        book_print_all(bookRoot);
                        break;

                    case 2:
                    {
                        int id;
                        printf("ID do livro: ");
                        if (!safe_read_int(&id)) break;

                        Book *book = book_find(bookRoot, id);

                        if (book)
                            print_book(book);
                        else
                            printf("Livro nao encontrado.\n");

                        break;
                    }

                    case 3:
                        printf("Emprestimo ainda nao implementado.\n");
                        break;

                    case 4:
                        printf("Meus emprestimos ainda nao implementado.\n");
                        break;

                    case 5:
                        printf("Reservas ainda nao implementado.\n");
                        break;

                    case 6:
                        printf("Historico ainda nao implementado.\n");
                        break;

                    case 0:
                        logout(session);
                        break;

                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
            }
        }
    }
}