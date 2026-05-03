# Casos de teste

## Tipo
| Entrada                         | Caso          | Regra |
| ------------------------------- | ------------- | ----- |
| int/float/char/double/char/void | Token de tipo | tipo  |

## Declaração

| Entrada           | Caso                                                   | Regra                       | Esperado          |
| ----------------- | ------------------------------------------------------ | --------------------------- | ----------------- |
| int x;            | Declaração sem inicializar                             | tipo declaradores SEMICOLON | Aceito            |
| int x, y;         | Declaração plural sem inicializar                      |                             | Aceito            |
| int x = 5;        | Declaração inicializando                               |                             | Aceito            |
| int x = 5, y = 6; | Declaração plural inicializando                        |                             | Aceito            |
| int x, y = 6;     | Declaração plural mista                                |                             | Aceito            |
| int               | Declarador sem ID                                      |                             | Falho             |
| int x             | Declaração sem ponto e vírgula                         |                             | Falho             |
| int x, y          | Declaração plural sem ponto e vírgula                  |                             | Falho             |
| int x =           | Declaração inicializando valor não dado                |                             | Falho             |
| int x,            | Declaração plural interrompida                         |                             | Falho             |
| int x, y =        | Declaração plural com inicialização de valor não dado  |                             | Falho             |
| int int           | Nome de tipo como Identificador                        |                             | Falho             |
| int x = 5.6;      | int inicializado a partir de float                     |                             | Questão semântica |
| float x = 5;      | float inicializado a partir de int                     |                             | Questão semântica |
| int x = "oi" + 1; | string em operação aritmética                          |                             | Questão semântica |
| int x = 'o' + 1;  | char em operação aritmética                            |                             | Questão semântica |
| int x = y;        | Variável não declarada sendo usada                     |                             | Questão semântica |
| int x; int x;     | Variável declarada 2 vezes                             |                             | Questão semântica |

## Atualização e Atribuição

| Entrada              | Caso                                           | Regra                             | Esperado          |
| -------------------- | ---------------------------------------------- | --------------------------------- | ----------------- |
| int x=0; x=5;        | Atribuição simples                             | ID INITVAR expressao SEMICOLON    | Aceito            |
| int x=0; x+=3;       | Atualização composta (+=)                      | ID UPDT_PLUS expressao SEMICOLON  | Aceito            |
| int x=0; x-=3;       | Atualização composta (-=)                      | ID UPDT_MINUS expressao SEMICOLON | Aceito            |
| int x=0; x*=2;       | Atualização composta (*=)                      | ID UPDT_TIMES expressao SEMICOLON | Aceito            |
| int x=0; x/=2;       | Atualização composta (/=)                      | ID UPDT_DIV expressao SEMICOLON   | Aceito            |
| int x=0; x++;        | Incremento                                     | ID UPDT_INC SEMICOLON             | Aceito            |
| int x=0; x--;        | Decremento                                     | ID UPDT_DEC SEMICOLON             | Aceito            |
| x = ;                | Atribuição sem valor                           |                                   | Falho             |
| x += ;               | Atualização sem valor (+=)                     |                                   | Falho             |
| x -= ;               | Atualização sem valor (-=)                     |                                   | Falho             |
| x *= ;               | Atualização sem valor (*=)                     |                                   | Falho             |
| x++ 3;               | Incremento com operando extra                  |                                   | Falho             |
| x-- 3;               | Decremento com operando extra                  |                                   | Falho             |
| int x=0; x="hello";  | Tipo incompatível na atribuição (string → int) |                                   | Questão semântica |
| x = 5;               | Variável não declarada na atribuição           |                                   | Questão semântica |
| int x=0; x=3.14;     | Tipo incompatível na atribuição (float → int)  |                                   | Questão semântica |
| int x=0; x+=y;       | Variável não declarada na atualização          |                                   | Questão semântica |

## Operadores Aritméticos

| Entrada      | Caso                                 | Regra                         | Esperado          |
| ------------ | ------------------------------------ | ----------------------------- | ----------------- |
| 3 + 5;       | Soma de inteiros                     | expressao PLUS expressao      | Aceito            |
| 3 + 5 * 2;   | Precedência de operadores            |                               | Aceito            |
| (3 + 5) * 2; | Precedência com parênteses           |                               | Aceito            |
| 10 % 3;      | Módulo de inteiros                   | expressao MOD expressao       | Aceito            |
| 10 // 3;     | Divisão inteira                      | expressao INTDIVIDE expressao | Aceito            |
| 3.0 + 1.5;   | Soma de floats                       |                               | Aceito            |
| 3 ++5        | Operador inválido entre operandos    |                               | Falho             |
| (3 + 5 * 2   | Parêntese não fechado                |                               | Falho             |
| 3 +          | Operando direito ausente             |                               | Falho             |
| 10 %         | Módulo sem operando direito          |                               | Falho             |
| 10 //        | Divisão inteira sem operando direito |                               | Falho             |
| "oi" + 1;    | String em operação aritmética        |                               | Questão semântica |
| x + 1;       | Variável não declarada em expressão  |                               | Questão semântica |
| 10 / 0;      | Divisão por zero                     |                               | Questão semântica |
| 10 % 0;      | Módulo por zero                      |                               | Questão semântica |