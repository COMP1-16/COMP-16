# Casos de teste

## Tipo
| Entrada                         | Caso          | Regra |
| ------------------------------- | ------------- | ----- |
| int/float/char/double/char/void | Token de tipo | tipo  |

## Declaração

| Entrada           | Caso                                                   | Regra                       | Esperado          |
| ----------------- | ------------------------------------------------------ | --------------------------- | ----------------- |
| int x;            | Declaração sem inicializar;                            | tipo declaradores SEMICOLON | Aceito            |
| int x, y;         | Declaração plural sem inicializar;                     |                             | Aceito            |
| int x = 5;        | Declaração inicializando;                              |                             | Aceito            |
| int x = 5, y = 6; | Declaração plural inicializando;                       |                             | Aceito            |
| int x, y = 6;     | Declaração plural mista;                               |                             | Aceito            |
| int               | Declarador sem ID;                                     |                             | Falho             |
| int x             | Declaração sem ponto e vírgula;                        |                             | Falho             |
| int x, y          | Declaração plural sem ponto e vírgula;                 |                             | Falho             |
| int x =           | Declaração inicializando valor não dado;               |                             | Falho             |
| int x,            | Declaração plural interrompida;                        |                             | Falho             |
| int x, y =        | Declaração plural com inicialização de valor não dado; |                             | Falho             |
| int int           | Nome de tipo como Identificador;                       |                             | Falho             |
| int x = 5.6;      | int inicia a partir de float;                          |                             | Questão semantica |
| float x = 5;      | float inicia a partir de int;                          |                             | Questão semântica |
| int x = "oi" + 1; | string em operação aritmética;                         |                             | Questão semântica |
| int x = 'o' + 1;  | char em operação aritmética;                           |                             | Questão semântica |
| int x = y;        | Variável não declarada sendo usada;                    |                             | Questão semântica |
| int x;<br>int x;  | Variável declarada 2 vezes;                            |                             | Questão semântica |

