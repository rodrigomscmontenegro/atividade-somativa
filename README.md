# Sistema de Manutenção de Contas

Sistema de cadastro e gerenciamento de contas bancárias desenvolvido em C, utilizando arquivo binário com registros de tamanho fixo.

## Funcionalidades

- Cadastrar novo cliente
- Consultar cliente pelo número da conta
- Atualizar saldo
- Encerrar conta
- Listar todos os clientes ativos
- Reiniciar leitura do arquivo com `rewind()`

## Estrutura do Projeto

```
├── main.c      # Menu principal e ponto de entrada
├── cliente.h   # Estrutura de dados e protótipos
├── cliente.c   # Implementação das funções
```

## Como Compilar e Rodar

### Windows (MinGW)

```bash
gcc -Wall -Wextra -o contas main.c cliente.c
contas.exe
```

### Linux / Mac

```bash
gcc -Wall -Wextra -o contas main.c cliente.c
./contas
```

## Conceitos Utilizados

| Função | Uso |
|--------|-----|
| `fseek()` | Acesso direto a posições do arquivo |
| `fread()` | Leitura de registros binários |
| `fwrite()` | Escrita de registros binários |
| `rewind()` | Retorna o ponteiro ao início do arquivo |

## Observações

- O arquivo `contas.dat` é criado automaticamente na primeira execução.
- Contas encerradas são removidas de forma lógica (campo `ativo = 0`), liberando o slot para reutilização futura.