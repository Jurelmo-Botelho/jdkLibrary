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

                    case 4:
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

                    case 9: //EMPRÉSTIMOS

                        printf("\n--- EFECTUAR EMPRÉSTIMO ---\n");
                        printf("\n--- Liros ---\n");
                        book_print_all(bookRoot);
                        printf("\n--- Utilizadores ---\n");
                        user_print_all(userRoot);

                        int userId, bookId;

                        printf("Inserir ID do Livro: ");
                        if (!safe_read_int(&bookId)) break;

                        printf("Inserir ID do Leitor: ");
                        if (!safe_read_int(&userId)) break; 

                        User *selectedUser = user_find(userRoot, userId);
                        Book *selectedBook = book_find(bookRoot, bookId);
                        if(!selectedUser || !selectedBook) {
                            printf("Usuário ou Livro não encontrado");
                            break;
                        }
                        if(!user_can_borrow(selectedUser) || !book_can_borrow(selectedBook, selectedUser->age))
                         break;

                        if (loan_find_by_user_and_book(globalLoans, userId, bookId)) {
                             printf("Utilizador já possui este livro emprestado.\n");
                            break;
                        }

                        if (selectedBook->availableQuantity == 0) {
                            printf("Livro indisponível no momento.\n");
                            printf("Deseja entrar na fila de reservas? (1-Sim / 0-Não): ");
                            int reserve;
                            if (safe_read_int(&reserve) && reserve == 1) {
                                 if (reserve_add(selectedBook, selectedUser)) {
                                    printf("Reserva realizada com sucesso!\n");
                                }
                            }
                            break;
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
                            printf("Erro ao criar empréstimo.\n");
                            break;
                        }

                        loan_add_to_global(globalLoans, newLoan);
                        loan_add_to_user(selectedUser, newLoan);
                            
                        selectedBook->availableQuantity--;
                        selectedBook->timesBorrowed++;

                        printf("\nEMPRÉSTIMO REALIZADO COM SUCESSO!\n");
                        printf("   ID do Empréstimo: %d\n", newLoan->id);
                        printf("   Livro: %s\n", selectedBook->title);
                        printf("   Usuário: %s\n", selectedUser->name);
                        printf("   Data do Empréstimo: %02d/%02d/%04d\n", 
                            today.day, today.month, today.year);
                        printf("   Data Prevista de Devolução: %02d/%02d/%04d\n", 
                            dateExpected.day, dateExpected.month, dateExpected.year);
                        printf("   Quantidade Disponível: %d\n", selectedBook->availableQuantity);
                        
    
                        printf(" Empréstimos Ativos do Usuário: %d/3\n", selectedUser->activeLoans);

                        break;

                   case 10:
                    printf("\n--- EFETUAR DEVOLUCAO ---\n");
                    printf("1. Devolver por ID do Livro e ID do Usuario\n");
                    printf("2. Devolver por ID do Emprestimo\n");
                    printf("3. Ver todos os emprestimos ativos\n");
                    printf("Opcao: ");
                    
                    int devOption;
                    if (!safe_read_int(&devOption)) break;
                    
                    switch(devOption) {
                        case 1: {
                            int bookId, userId;
                            printf("ID do Livro: ");
                            if (!safe_read_int(&bookId)) break;
                            printf("ID do Usuario: ");
                            if (!safe_read_int(&userId)) break;
                            process_book_return(globalLoans, history, bookRoot, bookId, userId);
                            break;
                        }
                        case 2: {
                            int loanId;
                            printf("ID do Emprestimo: ");
                            if (!safe_read_int(&loanId)) break;
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
                    break;

                    case 11:
                        printf("\n--- CONSULTAR RESERVAS ---\n");
                        printf("ID do Livro: ");
                        int searchBookId;
                        if (!safe_read_int(&searchBookId)) break;
                        
                        Book *searchBook = book_find(bookRoot, searchBookId);
                        if (searchBook) {
                            reserve_print_queue(searchBook);
                        } else {
                            printf("Livro não encontrado.\n");
                        }
                        break;

                    case 12:
                        loan_print_all_active(globalLoans);
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

                        printf("\n--- EFECTUAR EMPRÉSTIMO ---\n");
                        printf("\n--- Liros ---\n");
                        book_print_all(bookRoot);
                        printf("\n--- Utilizadores ---\n");
                        user_print_all(userRoot);

                        int userId = session->currentUser->id, bookId;

                        printf("Inserir ID do Livro: ");
                        if (!safe_read_int(&bookId)) break;

                        User *selectedUser = user_find(userRoot, userId);
                        Book *selectedBook = book_find(bookRoot, bookId);
                        if(!selectedUser || !selectedBook) {
                            printf("Usuário ou Livro não encontrado");
                            break;
                        }
                        if(!user_can_borrow(selectedUser) || !book_can_borrow(selectedBook, selectedUser->age))
                         break;

                        if (loan_find_by_user_and_book(globalLoans, userId, bookId)) {
                             printf("Utilizador já possui este livro emprestado.\n");
                            break;
                        }

                        if (selectedBook->availableQuantity == 0) {
                            printf("Livro indisponível no momento.\n");
                            printf("Deseja entrar na fila de reservas? (1-Sim / 0-Não): ");
                            int reserve;
                            if (safe_read_int(&reserve) && reserve == 1) {
                                 if (reserve_add(selectedBook, selectedUser)) {
                                    printf("Reserva realizada com sucesso!\n");
                                }
                            }
                            break;
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
                            printf("Erro ao criar empréstimo.\n");
                            break;
                        }

                        loan_add_to_global(globalLoans, newLoan);
                        loan_add_to_user(selectedUser, newLoan);
                            
                        selectedBook->availableQuantity--;
                        selectedBook->timesBorrowed++;

                        printf("\nEMPRÉSTIMO REALIZADO COM SUCESSO!\n");
                        printf("   ID do Empréstimo: %d\n", newLoan->id);
                        printf("   Livro: %s\n", selectedBook->title);
                        printf("   Usuário: %s\n", selectedUser->name);
                        printf("   Data do Empréstimo: %02d/%02d/%04d\n", 
                            today.day, today.month, today.year);
                        printf("   Data Prevista de Devolução: %02d/%02d/%04d\n", 
                            dateExpected.day, dateExpected.month, dateExpected.year);
                        printf("   Quantidade Disponível: %d\n", selectedBook->availableQuantity);
                        
    
                        printf(" Empréstimos Ativos do Usuário: %d/3\n", selectedUser->activeLoans);

                        break;

                    case 4:
                        printf("\n--- OS MEUS EMPRÉSTIMOS ---\n");
                         userId = session->currentUser->id;
                        if (userId) {
                            User *user = user_find(userRoot, userId);
                            if (user) {
                                loan_print_user_loans(user);
                            } else {
                                printf(" Usuário não encontrado.\n");
                            }
                        }
                        break;

                    case 5: 
                        printf("\n--- MINHAS RESERVAS ---\n");
                         userId = session->currentUser->id;
                        if (userId) {
                            User *user = user_find(userRoot, userId);
                            if (user) {
                                reserve_print_user_reservations(bookRoot, user);
                            } else {
                                printf("Usuario nao encontrado.\n");
                            }
                        }
                        break;

                    case 6:  
                        printf("\n--- MEU HISTORICO ---\n");
                          userId = session->currentUser->id;
                        if (userId) {
                            User *user = user_find(userRoot, userId);
                            if (user) {
                                loan_print_user_history(history, userId);
                            } else {
                                printf("Usuario nao encontrado.\n");
                            }
                        }
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