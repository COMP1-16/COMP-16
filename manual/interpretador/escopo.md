# Escopo do interpretador

> Lista do que o interpretador **implementa e testa** nesta versão. O contexto sobre a linguagem C e a organização desta seção da documentação está em [Linguagem e Escopo — visão geral](linguagem-e-escopo.md).

| Área | Implementado |
|------|--------------|
| Pipeline | Análise léxica, sintática, semântica, otimização e interpretação; flags `--parse-only`, `--semantic-only`, `--optimize-only` e `--no-opt` |
| Tipos e variáveis | `int`, `float`, `char`, `void` e `string` (extensão para literais de texto); declaração simples, com inicialização e múltipla (`int a, b;`); escopo local, sombreamento e detecção de redeclaração; comentários de linha (`//`) |
| Operadores aritméticos | `+`, `-`, `*`, `/`, `%` (incluindo menos unário) |
| Operadores relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Operadores lógicos | `&&`, `||`, `!` (com avaliação em curto-circuito) |
| Atribuição e atualização composta | `=`, `+=`, `-=`, `*=`, `/=` |
| Incremento e decremento | `++`, `--` |
| Controle de fluxo | `if`, `else if`, `else`; `while` e `for`; `switch`, `case`, `default` (com *fall-through* e `case` com `char`); `break`, `continue` e `return` |
| Funções | Definição com corpo, parâmetros tipados e retorno; chamada com passagem por valor; recursão; escopo de parâmetros e variáveis locais |
| Entrada e saída | `printf` com especificadores `%d`, `%i`, `%f`, `%s`, `%c` e sequências de escape (`\n`) |
| Biblioteca `math.h` | `sqrt`, `pow`, `abs`, `floor`, `ceil`, `round` |
| Biblioteca `stdlib.h` | `atoi`, `atof`, `exit` |
| Biblioteca `ctype.h` | `isalpha`, `isdigit`, `isspace` |
| Otimizador | *Constant folding* em expressões aritméticas, relacionais e lógicas; dobra de chamadas a `math.h`, `stdlib.h` e `ctype.h` com literais; simplificação de atribuições compostas; eliminação de código morto em `if`, `while`, `for` e após `return`, `break` e `continue` |
| Testes automatizados | **389 casos** em três camadas (sintática, semântica e execução) para declaração/atualização de variáveis, operadores, `if/else`, `while`, `for`, `switch/case`, recursão, `math.h`, `stdlib.h`, `ctype.h` e otimizador |

!!! warning "Fora do escopo implementado"
    Ainda **não** implementados: `do while`, `scanf`, `const`, arrays, ponteiros, `double`, `bool`, `string.h`, `rand`/`srand` e protótipos de função.

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 18/04/26 | Criação da página inicial | Pedro Henrique |
| 1.1 | 13/05/26 | Revisão do conteúdo e da estética da página | Camila Careli |
| 1.2 | 27/06/26 | Atualização para refletir o escopo realmente implementado e testado | Equipe COMP-16 |
