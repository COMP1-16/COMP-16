# Escopo do Interpretador de C

---

## Tipos e Variáveis  

- Tipos primitivos: `int`, `float`, `double`, `char`, `bool`  
- Declaração com e sem inicialização  
- Modificador `const`  
- Arrays unidimensionais de tipos primitivos  
- Strings como `char[]` / `char*`  

---

## Operadores  

| Categoria | Operadores |
|----------|-----------|
| Aritméticos | `+`, `-`, `*`, `/`, `%` |
| Relacionais | `==`, `!=`, `<`, `>`, `<=`, `>=` |
| Lógicos | `&&`, `||`, `!` |
| Atribuição | `=`, `+=`, `-=`, `*=`, `/=` |
| Incremento/Decremento | `++`, `--` |

---

## Controle de Fluxo  

- `if / else if / else`  
- `while`, `do while`, `for`  
- `switch / case / default`  
- `break`, `continue`, `return`  
- Caso especial: `return 0`  

---

## Funções  

- Declaração e chamada com parâmetros e retorno  
- Recursão  
- Escopo de variáveis locais  

---

## Biblioteca Padrão (subconjunto)  

### stdio.h  
- `printf`, `scanf`  

### math.h  
- `sqrt`, `pow`, `abs`, `floor`, `ceil`, `round`  

### string.h  
- `strlen`, `strcpy`, `strcmp`, `strcat`, `strchr`  

### stdlib.h  
- `abs`, `rand`, `srand`, `atoi`, `atof`, `exit`  

### ctype.h  
- `isalpha`, `isdigit`, `isspace`  