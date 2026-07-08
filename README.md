# jdkLibrary

## Sistema de Gestão de Biblioteca

## Descrição do Projeto

Sistema de gestão de biblioteca desenvolvido em C como projeto para a disciplina de Estruturas de Dados. O sistema permite a gestão completa de livros, leitores, empréstimos, devoluções, reservas e geração de relatórios, demonstrando o domínio dos principais conceitos de estruturas de dados.

## Estruturas de Dados Utilizadas

### Árvore AVL Genérica
Implementação única e reutilizável de árvore AVL com ponteiro genérico (void *), permitindo a mesma implementação para diferentes tipos de dados. A árvore armazena elementos através de uma chave inteira e utiliza funções de comparação específicas para cada tipo de dado.

- Árvore AVL de Livros (organizada por ID)
- Árvore AVL de Leitores (organizada por ID)

### Listas Ligadas
- Lista global de empréstimos ativos
- Lista de empréstimos por leitor
- Histórico de empréstimos

### Filas
- Fila de reservas por livro (ordem FIFO)

## Funcionalidades

### Gestão de Livros
- Registar livro
- Atualizar livro
- Remover livro
- Consultar todos os livros
- Procurar livro por ID

### Gestão de Leitores
- Registar leitor
- Atualizar leitor
- Remover leitor
- Consultar todos os leitores
- Sistema de login e autenticação

### Gestão de Empréstimos
- Efetuar empréstimo (com validação de idade e limite de 3 livros)
- Efetuar devolução
- Consultar empréstimos ativos
- Consultar histórico de empréstimos

### Gestão de Reservas
- Entrar na fila de reservas (quando livro indisponível)
- Consultar fila de reservas de um livro
- Processamento automático de reservas após devolução

### Relatórios
- Livros mais requisitados
- Livros menos requisitados
- Livros nunca requisitados
- Livros disponíveis
- Livros indisponíveis
- Leitores mais ativos
- Leitores com mais empréstimos ativos
- Leitores sem empréstimos
- Leitores com empréstimos em atraso
- Totais da biblioteca

### Persistência de Dados
- Salvamento automático ao sair do programa
- Carregamento automático ao iniciar o programa
- Arquivos: users.txt, books.txt, loans.txt, history.txt, reservations.txt, report.txt

## Regras de Negócio

### Livros
- ID único
- Não é permitido remover livro com empréstimos ativos
- Quantidade disponível não pode ser superior à quantidade total
- Quantidade total não pode ser inferior ao número de exemplares emprestados

### Leitores
- ID único
- Username único
- Não é permitido remover leitor com empréstimos ativos

### Empréstimos
- Máximo de 3 livros por leitor
- Apenas livros disponíveis podem ser emprestados
- Verificação de idade mínima exigida pelo livro
- Devolução atualiza automaticamente a disponibilidade

### Reservas
- Leitor não pode reservar livro que já possui emprestado
- Leitor não pode reservar o mesmo livro mais de uma vez
- Reserva respeita ordem FIFO

## Como Compilar e Executar

### Pré-requisitos
- Compilador GCC
- Sistema operacional Linux/Unix ou Windows com MinGW

### Compilação
- gcc main.c src/*.c -Iinclude -o biblioteca 

## Authors: 
- Daniel Filipe | Jurelmo Botelho | Kenny Marques
