#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"   /* sempre inclui o proprio header primeiro */

long total_registros(FILE *fp)
{
    fseek(fp, 0, SEEK_END);
    return ftell(fp) / (long)sizeof(Cliente);
}

void exibir_cliente(const Cliente *c, int posicao){
    printf("\n+---------------------------------------+\n");
    printf("|  Posicao no arquivo : %-16d |\n", posicao);
    printf("|  Numero da conta    : %-16d |\n", c->numero_conta);
    printf("|  Nome               : %-16s |\n", c->nome);
    printf("|  Saldo              : R$ %13.2f |\n", c->saldo);
    printf("|  Status             : %-16s |\n", c->ativo ? "Ativa" : "Encerrada");
    printf("+---------------------------------------+\n");
}

/* Descarta o restante da linha do stdin */
void limpar_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/* Aguarda o usuario pressionar Enter */
void pausar(void)
{
    printf("\nPressione Enter para continuar...");
    getchar();
}

void cadastrar_cliente(FILE *fp)
{
    Cliente c, novo;
    long total, i, posicao = -1;

    total = total_registros(fp);

    /* Procura o primeiro slot inativo para reaproveitar */
    rewind(fp);
    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (!c.ativo) {
            posicao = i;
            break;
        }
    }
    /* Se nao achou slot vago, insere no final */
    if (posicao == -1)
        posicao = total;

    printf("\n--- CADASTRAR NOVO CLIENTE ---\n");

    printf("Numero da conta : ");
    scanf("%d", &novo.numero_conta);
    limpar_buffer();

    /* Verifica se o numero de conta ja existe */
    rewind(fp);
    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.numero_conta == novo.numero_conta) {
            printf("Erro: ja existe uma conta ativa com o numero %d.\n",
                   novo.numero_conta);
            pausar();
            return;
        }
    }

    printf("Nome do cliente : ");
    fgets(novo.nome, NOME_TAM, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Saldo inicial   : R$ ");
    scanf("%lf", &novo.saldo);
    limpar_buffer();

    novo.ativo = 1;

    fseek(fp, posicao * sizeof(Cliente), SEEK_SET);
    if (fwrite(&novo, sizeof(Cliente), 1, fp) == 1) {
        fflush(fp);
        printf("\nCliente cadastrado com sucesso!\n");
        printf("  Conta  : %d\n", novo.numero_conta);
        printf("  Nome   : %s\n", novo.nome);
        printf("  Saldo  : R$ %.2f\n", novo.saldo);
    } else {
        printf("Erro ao gravar o registro.\n");
    }

    pausar();
}

/* 2. Consultar cliente pelo numero da conta */
void consultar_cliente(FILE *fp)
{
    Cliente c;
    int numero, encontrado = 0;
    long total, i;

    printf("\n--- CONSULTAR CLIENTE ---\n");
    printf("Numero da conta: ");
    scanf("%d", &numero);
    limpar_buffer();

    total = total_registros(fp);
    rewind(fp);

    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.numero_conta == numero) {
            exibir_cliente(&c, (int)i);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Conta %d nao encontrada ou encerrada.\n", numero);

    pausar();
}

/* 3. Atualizar o saldo de um cliente */
void atualizar_saldo(FILE *fp)
{
    Cliente c;
    int numero, encontrado = 0;
    long total, i;
    double novo_saldo;

    printf("\n--- ATUALIZAR SALDO ---\n");
    printf("Numero da conta: ");
    scanf("%d", &numero);
    limpar_buffer();

    total = total_registros(fp);
    rewind(fp);

    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.numero_conta == numero) {
            printf("Cliente    : %s\n", c.nome);
            printf("Saldo atual: R$ %.2f\n", c.saldo);
            printf("Novo saldo : R$ ");
            scanf("%lf", &novo_saldo);
            limpar_buffer();

            c.saldo = novo_saldo;

            /* Volta para a posicao deste registro e regrava */
            fseek(fp, i * sizeof(Cliente), SEEK_SET);
            if (fwrite(&c, sizeof(Cliente), 1, fp) == 1) {
                fflush(fp);
                printf("Saldo atualizado com sucesso!\n");
            } else {
                printf("Erro ao atualizar o saldo.\n");
            }

            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Conta %d nao encontrada ou encerrada.\n", numero);

    pausar();
}

/* 4. Encerrar conta (remocao logica: ativo = 0) */
void encerrar_conta(FILE *fp)
{
    Cliente c;
    int numero, encontrado = 0;
    long total, i;
    char confirmacao;

    printf("\n--- ENCERRAR CONTA ---\n");
    printf("Numero da conta: ");
    scanf("%d", &numero);
    limpar_buffer();

    total = total_registros(fp);
    rewind(fp);

    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo && c.numero_conta == numero) {
            exibir_cliente(&c, (int)i);
            printf("Confirmar encerramento? (s/n): ");
            scanf(" %c", &confirmacao);
            limpar_buffer();

            if (confirmacao == 's' || confirmacao == 'S') {
                c.ativo = 0;
                fseek(fp, i * sizeof(Cliente), SEEK_SET);
                if (fwrite(&c, sizeof(Cliente), 1, fp) == 1) {
                    fflush(fp);
                    printf("Conta %d encerrada com sucesso.\n", numero);
                } else {
                    printf("Erro ao encerrar a conta.\n");
                }
            } else {
                printf("Operacao cancelada.\n");
            }

            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Conta %d nao encontrada ou ja encerrada.\n", numero);

    pausar();
}

/* 5. Listar todos os clientes ativos */
void listar_clientes(FILE *fp)
{
    Cliente c;
    long total, i;
    int ativos = 0;

    printf("\n--- LISTA DE CLIENTES ATIVOS ---\n");
    printf("%-6s %-10s %-30s %12s\n", "Pos.", "Conta", "Nome", "Saldo (R$)");
    printf("%-6s %-10s %-30s %12s\n",
           "------", "----------", "------------------------------", "------------");

    total = total_registros(fp);
    rewind(fp);

    for (i = 0; i < total; i++) {
        fread(&c, sizeof(Cliente), 1, fp);
        if (c.ativo) {
            printf("%-6ld %-10d %-30s %12.2f\n",
                   i, c.numero_conta, c.nome, c.saldo);
            ativos++;
        }
    }

    if (ativos == 0)
        printf("Nenhum cliente ativo encontrado.\n");
    else
        printf("\nTotal de clientes ativos: %d\n", ativos);

    pausar();
}

/* 6. Reiniciar leitura do arquivo com rewind() */
void reiniciar_leitura(FILE *fp)
{
    printf("\n--- REINICIAR LEITURA DO ARQUIVO ---\n");
    rewind(fp);
    printf("Ponteiro retornado ao inicio do arquivo com rewind().\n");
    printf("Listagem a seguir:\n\n");
    listar_clientes(fp);   /* ja chama pausar() internamente */
}