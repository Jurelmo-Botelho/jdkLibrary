#include <stdio.h>
#include "ui.h"

void mostrarMenuPrincipal()
{
    printf("\n=====================================\n");
    printf(" SISTEMA DE GESTAO DE BIBLIOTECA\n");
    printf("=====================================\n");
    printf("1. Login\n");
    printf("0. Sair\n");
    printf("Opcao: ");
}

void mostrarMenuAdmin()
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

void mostrarMenuUtilizador()
{
    printf("\n=====================================\n");
    printf(" MENU UTILIZADOR\n");
    printf("=====================================\n");
    printf("1. Procurar Livro\n");
    printf("2. Requisitar Livro\n");
    printf("3. Reservar Livro\n");
    printf("4. Logout\n");
    printf("Opcao: ");
}

void mostrarMenuLivros()
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

void mostrarMenuRelatorios()
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