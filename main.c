#include <stdio.h>
#include <stdlib.h>
#include "cliente.h"   /* struct Cliente + prototipos */

static void menu(void)
{
    printf("\n");
    printf("+------------------------------------------+\n");
    printf("|    SISTEMA DE MANUTENCAO DE CONTAS       |\n");
    printf("+------------------------------------------+\n");
    printf("|  1. Cadastrar novo cliente               |\n");
    printf("|  2. Consultar cliente por numero         |\n");
    printf("|  3. Atualizar saldo                      |\n");
    printf("|  4. Encerrar conta (remover cliente)     |\n");
    printf("|  5. Listar todos os clientes             |\n");
    printf("|  6. Reiniciar leitura (rewind)           |\n");
    printf("|  7. Sair                                 |\n");
    printf("+------------------------------------------+\n");
}

int main(void)
{
    FILE *fp;
    int opcao;

    /* Tenta abrir o arquivo existente; se nao existir, cria um novo */
    fp = fopen(ARQUIVO, "r+b");
    if (fp == NULL) {
        fp = fopen(ARQUIVO, "w+b");
        if (fp == NULL) {
            fprintf(stderr, "Erro: nao foi possivel abrir/criar '%s'.\n", ARQUIVO);
            return EXIT_FAILURE;
        }
        printf("Arquivo '%s' criado com sucesso.\n\n", ARQUIVO);
    }

    do {
        menu();
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limpar_buffer();
            opcao = 0;
        }
        limpar_buffer();

        switch (opcao) {
            case 1: cadastrar_cliente(fp);  break;
            case 2: consultar_cliente(fp);  break;
            case 3: atualizar_saldo(fp);    break;
            case 4: encerrar_conta(fp);     break;
            case 5: listar_clientes(fp);    break;
            case 6: reiniciar_leitura(fp);  break;
            case 7:
                printf("\nEncerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("\nOpcao invalida. Tente novamente.\n");
                pausar();
        }
    } while (opcao != 7);

    fclose(fp);
    return EXIT_SUCCESS;
}