#ifndef CLIENTE_H   
#define CLIENTE_H

#include <stdio.h>

#define NOME_TAM  50
#define ARQUIVO   "contas.dat"

typedef struct {
    int    numero_conta;
    char   nome[NOME_TAM];
    double saldo;
    int    ativo;   /* 1 = conta ativa, 0 = encerrada */
} Cliente;

void cadastrar_cliente(FILE *fp);
void consultar_cliente(FILE *fp);
void atualizar_saldo  (FILE *fp);
void encerrar_conta   (FILE *fp);
void listar_clientes  (FILE *fp);
void reiniciar_leitura(FILE *fp);

long total_registros (FILE *fp);
void exibir_cliente  (const Cliente *c, int posicao);
void limpar_buffer   (void);
void pausar          (void);

#endif /* CLIENTE_H */