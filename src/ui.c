#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "auth.h"
#include "validation.h"
#include "book.h"
#include "user.h"
#include "date.h"
#include "data_persistence.h"
#include "loan.h"
#include "utils.h"
#include "report.h"

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
    printf("10. Fazer Devolucao\n");
    printf("11. Consultar Reservas\n");
    printf("12. Consultar Empréstimos\n");
    printf("13. Relatorios\n");
    printf("14. Historico\n");
    printf("0. Logout\n");
    printf("Opcao: ");
}

static void handle_admin_register_book(AVLNode **bookRoot)
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
    if (!safe_read_int(&year)) return;

    printf("Idade minima: ");
    if (!safe_read_int(&minAge)) return;

    printf("Quantidade: ");
    if (!safe_read_int(&qty)) return;

    register_book(bookRoot, title, author, publisher, category, year, minAge, qty);
}

static void handle_admin_update_book(AVLNode *bookRoot)
{
    int id;
    printf("ID do livro: ");
    if (!safe_read_int(&id)) return;

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
    if (!safe_read_int(&minAge)) return;

    update_book(bookRoot, id, title, author, category, minAge);
}

static void handle_admin_delete_book(AVLNode **bookRoot)
{
    int id;
    printf("ID do livro: ");
    if (!safe_read_int(&id)) return;
    delete_book(bookRoot, id);
}

static void handle_admin_register_user(AVLNode **userRoot)
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
    if (!safe_read_int(&age)) return;

    printf("Telefone: ");
    scanf("%s", phone);

    register_user(userRoot, username, password, name, age, phone, ROLE_STUDENT);
}

static void handle_admin_update_user(AVLNode *userRoot)
{
    int id;
    printf("ID do utilizador: ");
    if (!safe_read_int(&id)) return;

    char name[MAX_NAME];
    int age;
    char phone[MAX_PHONE];

    printf("Novo nome: ");
    scanf(" %[^\n]", name);

    printf("Nova idade: ");
    if (!safe_read_int(&age)) return;

    printf("Novo telefone: ");
    scanf("%s", phone);

    update_user(userRoot, id, name, age, phone);
}

static void handle_admin_delete_user(AVLNode **userRoot)
{
    int id;
    printf("ID do utilizador: ");
    if (!safe_read_int(&id)) return;
    delete_user(userRoot, id);
}

static void handle_loan_creation(AVLNode *bookRoot, AVLNode *userRoot, 
                                  LoanList *globalLoans, int userId, int isAdmin)
{
    int bookId;
    int selectedUserId = userId;
    
    printf("\n--- EFECTUAR EMPRESTIMO ---\n");
    printf("\n--- Livros ---\n");
    book_print_all(bookRoot);
    
    if (isAdmin) {
        printf("\n--- Utilizadores ---\n");
        user_print_all(userRoot);
        printf("\nInserir ID do Leitor: ");
        if (!safe_read_int(&selectedUserId)) return;
    }
    
    printf("Inserir ID do Livro: ");
    if (!safe_read_int(&bookId)) return;

    User *selectedUser = user_find(userRoot, selectedUserId);
    Book *selectedBook = book_find(bookRoot, bookId);
    
    if(!selectedUser || !selectedBook) {
        printf("Usuario ou Livro nao encontrado\n");
        return;
    }
    
    if(!user_can_borrow(selectedUser) || !book_can_borrow(selectedBook, selectedUser->age))
        return;

    if (loan_find_by_user_and_book(globalLoans, selectedUserId, bookId)) {
        printf("Utilizador ja possui este livro emprestado.\n");
        return;
    }

    if (selectedBook->availableQuantity == 0) {
        printf("Livro indisponivel no momento.\n");
        printf("Deseja entrar na fila de reservas? (1-Sim / 0-Nao): ");
        int reserve;
        if (safe_read_int(&reserve) && reserve == 1) {
            if (reserve_add(selectedBook, selectedUser)) {
                printf("Reserva realizada com sucesso!\n");
            }
        }
        return;
    }

    Date today = date_today();
    Date dateExpected = date_add_days(today, 15);

    Loan *newLoan = loan_create(
        globalLoans->quantity + 1,
        selectedBook,
        selectedUser,
        today,
        dateExpected
    );

    if (!newLoan) {
        printf("Erro ao criar emprestimo.\n");
        return;
    }

    loan_add_to_global(globalLoans, newLoan);
    loan_add_to_user(selectedUser, newLoan);
    
    selectedBook->availableQuantity--;
    selectedBook->timesBorrowed++;

    printf("\nEMPRESTIMO REALIZADO COM SUCESSO!\n");
    printf("   ID do Emprestimo: %d\n", newLoan->id);
    printf("   Livro: %s\n", selectedBook->title);
    printf("   Usuario: %s\n", selectedUser->name);
    printf("   Data do Emprestimo: %02d/%02d/%04d\n", 
        today.day, today.month, today.year);
    printf("   Data Prevista de Devolucao: %02d/%02d/%04d\n", 
        dateExpected.day, dateExpected.month, dateExpected.year);
    printf("   Quantidade Disponível: %d\n", selectedBook->availableQuantity);
    printf("   Emprestimos Ativos do Usuario: %d/3\n", selectedUser->activeLoans);
}

static void handle_return_book(LoanList *globalLoans, HistoryList *history, AVLNode *bookRoot)
{
    printf("\n--- EFETUAR DEVOLUCAO ---\n");
    printf("1. Devolver por ID do Livro e ID do Usuario\n");
    printf("2. Devolver por ID do Emprestimo\n");
    printf("3. Ver todos os emprestimos ativos\n");
    printf("Opcao: ");
    
    int devOption;
    if (!safe_read_int(&devOption)) return;
    
    switch(devOption) {
        case 1: {
            int bookId, userId;
            printf("ID do Livro: ");
            if (!safe_read_int(&bookId)) return;
            printf("ID do Usuario: ");
            if (!safe_read_int(&userId)) return;
            process_book_return(globalLoans, history, bookRoot, bookId, userId);
            break;
        }
        case 2: {
            int loanId;
            printf("ID do Emprestimo: ");
            if (!safe_read_int(&loanId)) return;
            Loan *loan = loan_find_by_id(globalLoans, loanId);
            if (loan) {
                process_book_return(globalLoans, history, bookRoot, 
                                  loan->book->id, loan->leitor->id);
            } else {
                printf("Emprestimo nao encontrado.\n");
            }
            break;
        }
        case 3: {
            loan_list_active_for_return(globalLoans);
            break;
        }
        default:
            printf("Opcao invalida.\n");
    }
}

static void handle_admin_reservations(AVLNode *bookRoot)
{
    printf("\n--- CONSULTAR RESERVAS ---\n");
    printf("ID do Livro: ");
    int searchBookId;
    if (!safe_read_int(&searchBookId)) return;
    
    Book *searchBook = book_find(bookRoot, searchBookId);
    if (searchBook) {
        reserve_print_queue(searchBook);
    } else {
        printf("Livro nao encontrado.\n");
    }
}

static void handle_admin_report(AVLNode *bookRoot, AVLNode *userRoot, LoanList *globalLoans, HistoryList *history)
{
    printf("\n--- Relatorio ---\n");
    printf("Gerando Relatorio...\n");
    ReportData *report = report_generate(bookRoot, userRoot, globalLoans, history);
    if (report) {
        report_print(report);
        printf("\nSalvar ? (1-sim / 0-Nao): ");
        int save;
        if (safe_read_int(&save) && save == 1) {
            report_save_to_file(report, "data/report.txt");
        }
        report_free(report);
    } else {
        printf("Erro ao Gerar o Relatorio.\n");
    }
}

static void handle_user_my_loans(AVLNode *userRoot, int userId)
{
    printf("\n--- OS MEUS EMPRESTIMOS ---\n");
    User *user = user_find(userRoot, userId);
    if (user) {
        loan_print_user_loans(user);
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

static void handle_user_my_reservations(AVLNode *bookRoot, AVLNode *userRoot, int userId)
{
    printf("\n--- MINHAS RESERVAS ---\n");
    User *user = user_find(userRoot, userId);
    if (user) {
        reserve_print_user_reservations(bookRoot, user);
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

static void handle_user_my_history(HistoryList *history, AVLNode *userRoot, int userId)
{
    printf("\n--- MEU HISTORICO ---\n");
    User *user = user_find(userRoot, userId);
    if (user) {
        loan_print_user_history(history, userId);
    } else {
        printf("Usuario nao encontrado.\n");
    }
}

static void handle_user_search_book(AVLNode *bookRoot)
{
    int id;
    printf("ID do livro: ");
    if (!safe_read_int(&id)) return;

    Book *book = book_find(bookRoot, id);
    if (book)
        print_book(book);
    else
        printf("Livro nao encontrado.\n");
}

static void handle_register_account(AVLNode **userRoot)
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
    if (!safe_read_int(&age)) return;

    printf("Telefone: ");
    scanf("%s", phone);

    register_user(userRoot, username, password, name, age, phone, ROLE_STUDENT);
}

static void handle_login(Session *session, AVLNode *userRoot)
{
    char username[MAX_USERNAME];
    char password[MAX_PASSWORD];

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    login(session, userRoot, username, password);
}

void run_system(void)
{
    AVLNode *userRoot = NULL;
    AVLNode *bookRoot = NULL;
    LoanList *globalLoans = (LoanList *)malloc(sizeof(LoanList));
    if (globalLoans != NULL) {
        globalLoans->head = NULL;
        globalLoans->tail = NULL;
        globalLoans->quantity = 0;
    }
    HistoryList *history = (HistoryList*)malloc(sizeof(HistoryList));
    history->head = NULL;
    history->tail = NULL;
    history->quantity = 0;

    create_default_admin(&userRoot);

    Session *session = create_session();

    load_all_data(&userRoot, &bookRoot, &globalLoans, &history);

    int option;

    while (1)
    {
        if (!session->isLoggedIn)
        {
            show_main_menu();

            if (!safe_read_int(&option))
                continue;

            switch (option)
            {
                case 1:
                    handle_login(session, userRoot);
                    break;

                case 2:
                    handle_register_account(&userRoot);
                    break;

                case 0:
                    printf("A sair do sistema...\n");
                    save_all_data(userRoot, bookRoot, globalLoans, history);
                    avl_destroy(userRoot);
                    avl_destroy(bookRoot);
                    loan_free_global(globalLoans);
                    loan_free_history(history);
                    destroy_session(session);
                    return;

                default:
                    printf("Opcao invalida.\n");
                    break;
            }
        }
        else
        {
            if (session->currentUser->role == ROLE_ADMIN)
            {
                show_admin_menu();

                if (!safe_read_int(&option))
                    continue;

                switch (option)
                {
                    case 1:
                        handle_admin_register_book(&bookRoot);
                        break;

                    case 2:
                        handle_admin_update_book(bookRoot);
                        break;

                    case 3:
                        handle_admin_delete_book(&bookRoot);
                        break;

                    case 4:
                        printf("\n--- LIVROS ---\n");
                        book_print_all(bookRoot);
                        break;

                    case 5:
                        handle_admin_register_user(&userRoot);
                        break;

                    case 6:
                        handle_admin_update_user(userRoot);
                        break;

                    case 7:
                        handle_admin_delete_user(&userRoot);
                        break;

                    case 8:
                        printf("\n--- UTILIZADORES ---\n");
                        user_print_all(userRoot);
                        break;

                    case 9:
                        handle_loan_creation(bookRoot, userRoot, globalLoans, 0, 1);
                        break;

                    case 10:
                        handle_return_book(globalLoans, history, bookRoot);
                        break;

                    case 11:
                        handle_admin_reservations(bookRoot);
                        break;

                    case 12:
                        loan_print_all_active(globalLoans);
                        break;

                    case 13:
                        handle_admin_report(bookRoot, userRoot, globalLoans, history);
                        break;

                    case 14:
                        printf("\n--- HISTORICO COMPLETO ---\n");
                        loan_print_all_history(history);
                        break;

                    case 0:
                        logout(session);
                        break;

                    default:
                        printf("Opcao invalida.\n");
                        break;
                }
            }
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
                        handle_user_search_book(bookRoot);
                        break;

                    case 3:
                        handle_loan_creation(bookRoot, userRoot, globalLoans, session->currentUser->id, 0);
                        break;

                    case 4:
                        handle_user_my_loans(userRoot, session->currentUser->id);
                        break;

                    case 5:
                        handle_user_my_reservations(bookRoot, userRoot, session->currentUser->id);
                        break;

                    case 6:
                        handle_user_my_history(history, userRoot, session->currentUser->id);
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