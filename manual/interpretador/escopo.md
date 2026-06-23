# Escopo do interpretador

> Lista objetiva do que o interpretador pretende cobrir neste projeto. O contexto sobre a linguagem C e a organização desta seção da documentação está em [Linguagem e Escopo — visão geral](linguagem-e-escopo.md).  
> Para ver o que já está implementado e testado hoje, consulte [Status do escopo](escopo-status.md).

| Área | O que entra no interpretador |
|------|------------------------------|
| Tipos e variáveis | Tipos primitivos `int`, `float`, `double`, `char`, `bool`; declaração com e sem inicialização; modificador `const`; arrays unidimensionais de tipos primitivos; strings como `char[]` / `char*` |
| Operadores aritméticos | `+`, `-`, `*`, `/`, `%` |
| Operadores relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Operadores lógicos | `&&`, `||`, `!` |
| Atribuição e atualização composta | `=`, `+=`, `-=`, `*=`, `/=` |
| Incremento e decremento | `++`, `--` |
| Controle de fluxo | `if`, `else if`, `else`; `while`, `do while`, `for`; `switch`, `case`, `default`; `break`, `continue`, `return` (incluindo `return 0`) |
| Funções | Declaração e chamada com parâmetros e retorno; recursão; escopo de variáveis locais |
| Biblioteca `stdio.h` | `printf`, `scanf` |
| Biblioteca `math.h` | `sqrt`, `pow`, `abs`, `floor`, `ceil`, `round` |
| Biblioteca `string.h` | `strlen`, `strcpy`, `strcmp`, `strcat`, `strchr` |
| Biblioteca `stdlib.h` | `abs`, `rand`, `srand`, `atoi`, `atof`, `exit` |
| Biblioteca `ctype.h` | `isalpha`, `isdigit`, `isspace` |

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 18/04/26 | Criação da página inicial | Pedro Henrique |
| 1.1 | 13/05/26 | Revisão do conteúdo e da estética da página | Camila Careli |