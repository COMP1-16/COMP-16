# Casos de Teste

> Esta se├º├úo descreve os casos de teste do interpretador, organizados por categoria de funcionalidade e fase de an├ílise (sint├ítico, sem├óntico e execu├º├úo). Cada caso inclui a entrada utilizada e, quando aplic├ível, a sa├¡da esperada ou o c├│digo de sa├¡da.

## Sobre os n├║meros

H├í **361 casos de teste** no reposit├│rio ÔÇö s├úo os arquivos `.txt` espalhados pelas 12 categorias
descritas abaixo. Quando voc├¬ roda `make test`, por├®m, o terminal mostra **421 verifica├º├Áes**.
N├úo ├® erro: alguns casos passam por mais de uma checagem.

Na pr├ítica, quase todo `.txt` roda **uma vez**. S├│ o `invalido_recursao_infinita.txt` fica de fora
(o script ignora esse arquivo para n├úo entrar em loop infinito). Isso d├í **360 execu├º├Áes** no total.

O que aumenta a conta s├úo os testes de **execu├º├úo v├ílidos** (`execucao/validos`). S├úo **115** casos
espalhados pelas categorias. Depois que o interpretador roda, o `tests.py` pode conferir de novo o
resultado ÔÇö mas s├│ quando existe arquivo auxiliar na mesma pasta:

- **59** casos t├¬m um `.expected` e a sa├¡da do programa ├® comparada com ele;
- **2** casos t├¬m um `.exitcode` e o c├│digo de retorno tamb├®m ├® verificado.

Essas **61** checagens extras se somam ├ás **360** execu├º├Áes normais: **360 + 61 = 421**. Os 115 casos
de `execucao/validos` j├í est├úo inclu├¡dos nas 360 ÔÇö as 61 n├úo s├úo arquivos novos, s├│ passagens a mais
do mesmo teste.

Para documenta├º├úo e relat├│rios, use **361 casos de teste**. O **421** aparece apenas na sa├¡da do
`make test`.

---

## Resumo Geral

| Categoria | Sint. V | Sint. I | Sem. V | Sem. I | Exec. V | Exec. I | Total |
|-----------|:-------:|:-------:|:------:|:------:|:-------:|:-------:|:-----:|
| Atualizacao de Variaveis | 4 | 3 | 3 | 3 | 10 | 5 | **28** |
| Declaracao de Variaveis | 4 | 3 | 5 | 5 | 10 | 5 | **32** |
| For | 5 | 5 | 5 | 4 | 10 | 5 | **34** |
| If/Else | 6 | 5 | 3 | 5 | 10 | 5 | **34** |
| Math | 1 | 5 | 1 | 2 | 6 | 2 | **17** |
| Operadores Aritmeticos | 3 | 4 | 2 | 3 | 10 | 5 | **27** |
| Operadores Logicos | 5 | 4 | 5 | 4 | 10 | 5 | **33** |
| Otimizador | 5 | 5 | 5 | 5 | 13 | 5 | **38** |
| Recursao | 5 | 5 | 5 | 4 | 9 | 5 | **33** |
| Stdlib | 1 | 5 | 1 | 2 | 8 | 2 | **19** |
| Switch/Case | 6 | 6 | 2 | 3 | 9 | 5 | **31** |
| While | 5 | 5 | 5 | 5 | 10 | 5 | **35** |
| **Total** | | | | | | | **361** |

> Legenda: **V** = V├ílidos, **I** = Inv├ílidos, **Sint.** = Sint├ítico, **Sem.** = Sem├óntico, **Exec.** = Execu├º├úo

---

## Atualizacao de Variaveis

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_atrib` | `float y = 5.0; y += 2.5;` |
| `valido_auto_inc` | `x += 1;` |
| `valido_inc` | `int x = 10; x++;` |
| `valido_mult` | `int z = 100; z *= 2;` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_falta_expr` | `int x = 10; x += ;` |
| `invalido_inc_valor` | `int z = 1; z++ 5;` |
| `invalido_op_invertido` | `int y = 5; y =+ 10;` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_coercao` | `int c = 10; float d = 1.0; d += c;` |
| `valido_float_inc` | `float b = 2.0; b++;` |
| `valido_soma` | `int a = 10; a += 5;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_atrib_incompativel` | `int k = 10; k = "erro";` |
| `invalido_nao_declarada` | `y += 10;` |
| `invalido_tipo_string` | `string s = "oi"; s++;` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_atrib_composta` | `int x = 5; x += 10; printf("%d", x);` | `15` |
| `valido_auto_inc_pos` | `int i = 1; i++; printf("%d", i);` | `2` |
| `valido_dce_pos_inc_loop` | `int i = 0; while (0) i++; i++;` | `ÔÇö` |
| `valido_fold_atrib_mult` | `int x = 2; x *= 3 + 4;` | `ÔÇö` |
| `valido_fold_atrib_soma` | `int x = 1; x += 2 + 3;` | `ÔÇö` |
| `valido_fold_atrib_sub` | `int x = 10; x -= 4 + 1;` | `ÔÇö` |
| `valido_inc_sequencia` | `int i = 0; i++; i++; i++; printf("%d", i);` | `3` |
| `valido_mult_atrib` | `int x = 3; x *= 4; printf("%d", x);` | `12` |
| `valido_noop_atrib_zero` | `int x = 5; x += 0;` | `ÔÇö` |
| `valido_sub_atrib` | `int x = 20; x -= 5; printf("%d", x);` | `15` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_atrib_nao_lvalue` | `5 = 10;` |
| `invalido_div_zero_literal` | `int x = 1 / 0; printf("%d", x);` |
| `invalido_inc_literal` | `++5;` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_var_runtime` | `printf("%d", z);` |

---

## Declaracao de Variaveis

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_decl` | `int a; float b = 1.1;` |
| `valido_decl_com_inicializacao_int` | `int x = 10;` |
| `valido_decl_com_inicializacao_string` | `int s = "nova_string";` |
| `valido_decl_composta` | `int x = 1, y = 2;` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_decl_string` | `char nome[2] = "ola mundo!";` |
| `invalido_nome` | `int 123var;` |
| `invalido_ponto_virgula` | `float x` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_decl_separada` | `int a; a = 5;` |
| `valido_string` | `string s = "UnB Gama";` |
| `valido_tipos` | `int x = 10; float y = 20.5;` |
| `valido_tipos_distintos` | `int x = 10; float y = 20.5;` |
| `valido_uso_var` | `int a; a = 5; int b = a;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_atrib_float_int` | `float p = 10.5; string s = p;` |
| `invalido_atrib_float_para_int` | `float preco = 10.5; string total = preco;` |
| `invalido_redeclaracao` | `int nota = 10; float nota = 9.5;` |
| `invalido_tipo_incompativel` | `int valor = "texto";` |
| `invalido_tipo_string` | `int valor = "texto";` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_char_init` | `char c = 'A'; printf("%d", c);` | `65` |
| `valido_decl_multipla_fold` | `int x = 10, y = 2 + 3;` | `ÔÇö` |
| `valido_decl_multipla_print` | `int x = 3, y = 4; printf("%d", x + y);` | `7` |
| `valido_decl_sem_uso` | `int x = 1 + 1;` | `ÔÇö` |
| `valido_fold_init_expr` | `int a = 1; int b = a + 4;` | `ÔÇö` |
| `valido_fold_init_int` | `int x = 2 + 3;` | `ÔÇö` |
| `valido_init_float_fold` | `float f = 1.0 + 2.0;` | `ÔÇö` |
| `valido_init_float_print` | `float b = 2.5; printf("%f", b);` | `2.500000` |
| `valido_init_int_print` | `int a = 10; printf("%d", a);` | `10` |
| `valido_string_print` | `string s = "hi"; printf("%s", s);` | `hi` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_float_para_int` | `int y = 2.7;` |
| `invalido_init_tipo` | `int x = "a";` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_redeclaracao` | `int x = 1; int x = 2;` |
| `invalido_var_nao_decl` | `printf("%d", n);` |

---

## For

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_for_condicao_vazia` | `for (i = 0; ; i++) { if (i>10) break; }` |
| `valido_for_corpo_sem_chaves` | `for (i = 0; i < 10; i++) a += i;` |
| `valido_for_corpo_vazio` | `for (i = 0; i < 10; i++) ;` |
| `valido_for_inicializacao_decl` | `for (int i = 0; i < 10; i++) { a = a + i; }` |
| `valido_for_inicializacao_vazia` | `for ( ; i < 10; i++) { }` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_for_inicializacao_sem_ponto_virgula` | `for (i = 0 i < 10; i++)` |
| `invalido_for_inicializacao_sem_ponto_virgula2` | `for (i = 0; i < 10 i++)` |
| `invalido_for_sem_corpo_com_decl_inicializacao` | `for (int i = 0; i < 10; i++)` |
| `invalido_for_sem_corpo_sem_decl_inicializacao` | `for (i = 0; i < 10; i++)` |
| `invalido_for_sem_parenteses` | `for i = 0; i < 10; i++` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_for_com_corpo_variaveis_decl` | `int soma = 0; for (int i = 1; i <= 10; i++) soma += i;` |
| `valido_for_condicao_tipos_compativeis` | `float x = 0.5; for (x; x < 10.0; x = x + 1.0) ;` |
| `valido_for_decl_corpo` | `for (int i = 0; i < 5; i = i + 1) { int j = i; }` |
| `valido_for_infinito` | `for ( ; ; ) break;` |
| `valido_for_sem_corpo_variaveis_decl` | `int i; for (i = 0; i < 10; i++) ;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_for_condicao_exp_relacional` | `for (int i = 0; i < "10"; i++) { }` |
| `invalido_for_condicao_string` | `for (i = 0; "texto"; i++) { }` |
| `invalido_for_condicao_variaveis_nao_decl_atualizacao` | `for (int i = 0; i < 1; j) { }` |
| `invalido_for_condicao_variaveis_nao_decl_inicializacao` | `for (i = 0; i < 10; i++) { }` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_for_break` | `int x = 0; for (int i = 0; i < 10; i++) { x = i; break; } printf("%d", x);` | `0` |
| `valido_for_break_dce` | `int x = 0; for (;;) { break; x = 5; }` | `ÔÇö` |
| `valido_for_cond_fold` | `int x = 0; for (int i = 0; i < 1 - 1; i++) x = 1;` | `ÔÇö` |
| `valido_for_contagem_regressiva` | `int n = 3; for (; n > 0; n--) { } printf("%d", n);` | `0` |
| `valido_for_corpo_morto` | `int x = 0; for (int i = 0; i < 0; i++) x = 99;` | `ÔÇö` |
| `valido_for_corpo_sem_chaves` | `int i = 0; for (i = 0; i < 2; i++) i++; printf("%d", i);` | `2` |
| `valido_for_inc_fold` | `int x = 0; for (int i = 0; i < 0; i = i + 1 + 1) x = 1;` | `ÔÇö` |
| `valido_for_init_decl` | `int s = 0; for (int i = 0; i < 2; i++) s++; printf("%d", s);` | `2` |
| `valido_for_init_fold` | `int x = 0; for (int i = 0 + 0; i < 0; i++) x = 1;` | `ÔÇö` |
| `valido_for_soma` | `int s = 0; for (int i = 0; i < 3; i++) s += i; printf("%d", s);` | `3` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_div_zero` | `for (int i = 0; i < 1; i++) { int x = 1 / 0; }` |
| `invalido_for_cond_string` | `for (int i = 0; "x"; i++) { }` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_var_corpo` | `for (int i = 0; i < 1; i++) { y = 1; }` |
| `invalido_var_nao_decl_init` | `for (j = 0; j < 1; j++) { }` |

---

## If/Else

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_dangling_else` | `if (x) if (y) { } else { }` |
| `valido_if_aninhado` | `if (x > 0) { if (y) { } }` |
| `valido_if_condicao_fixa` | `if (3) {     3+1; }` |
| `valido_if_condicao_variavel` | `if (x) { y = 1; }` |
| `valido_if_else` | `if ((3+5) == 8) {     return 1; } else {     return 0; }` |
| `valido_if_else_if` | `if ((3+5) > 8) {     return 2; } else if ((3+5) <  8) {     return 1; } else {     retu...` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_else_if_sem_condicao` | `if (x) { } else if { }` |
| `invalido_else_repetido` | `if (x) { } else else { }` |
| `invalido_else_sem_if` | `else { }` |
| `invalido_if` | `if x { }` |
| `invalido_if_sem_then_branch` | `if (x) else { }` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_else` | `int x=0; if(x){} else{x=1;}      // OK ÔÇö x acess├¡vel e mut├ível no else` |
| `valido_if` | `int x=1; if (x) { }  // 	OK ÔÇö inteiro usado como condi├º├úo (convers├úo impl├¡cita)` |
| `valido_if_com_declaracao` | `int x; if (x>0) {int y=x;}  // OK ÔÇö y declarado e usado dentro do escopo do bloco` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_else_if_string` | `if (1) { } else if ("a") { }` |
| `invalido_if_condicao_null` | `if (null) { }` |
| `invalido_if_condicao_string` | `if ("texto") { }` |
| `invalido_if_condicao_void` | `if ("texto") { }` |
| `invalido_if_var_nao_decl` | `if (x) { }` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_dangling_else` | `int x = 0; if (1) if (0) x = 1; else x = 2; printf("%d", x);` | `2` |
| `valido_else_morto` | `int x = 0; if (1) { x = 1; } else { x = 2; }` | `ÔÇö` |
| `valido_if_aninhado_fold` | `int x = 0; if (0) { if (1) { x = 1; } }` | `ÔÇö` |
| `valido_if_cond_fold` | `int x = 0; if (1 - 1) { x = 1; }` | `ÔÇö` |
| `valido_if_else` | `int x = 0; if (x) printf("sim"); else printf("nao");` | `nao` |
| `valido_if_else_if` | `int x = 2; if (x == 1) printf("1"); else if (x == 2) printf("2"); else printf...` | `2` |
| `valido_if_int_zero` | `int x = 0; if (x) printf("1"); else printf("0");` | `0` |
| `valido_if_then` | `int x = 1; if (x) printf("sim"); else printf("nao");` | `sim` |
| `valido_if_um_dce` | `int x = 1; if (1) { x = 2; } else { x = 99; }` | `ÔÇö` |
| `valido_if_zero_dce` | `int x = 1; if (0) { x = 99; } else { x = 2; }` | `ÔÇö` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_else_sem_if` | `else printf("x");` |
| `invalido_if_string` | `if ("a") printf("x");` |
| `invalido_if_void` | `if (void) printf("x");` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_var_nao_decl` | `if (x) printf("a");` |

---

## Math

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_include_math` | `#include <math.h>` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_func_sem_paren` | `#include <math.h> float x = sqrt 16.0;` |
| `invalido_include_malformado` | `#include <math.h` |
| `invalido_include_token` | `#include math.h>` |
| `invalido_pow_aberto` | `#include <math.h> float x = pow(2.0, 3.0;` |
| `invalido_sqrt_aberto` | `#include <math.h> float x = sqrt(16.0;` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_math_todas_funcoes` | `#include <math.h>  float a = sqrt(16.0); float b = pow(2.0, 3.0); float c = abs(-5.0); ...` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_math_funcao_sem_include` | `float a = sqrt(16.0); // Erro sem├óntico: sqrt n├úo foi declarado na tabela (falta o #inc...` |
| `invalido_math_parametros_faltando` | `#include <math.h>  float a = pow(2.0); // Erro sem├óntico/Runtime: pow espera 2 argument...` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_abs_literal` | `#include <math.h> float x = abs(-5.0);` | `ÔÇö` |
| `valido_floor_ceil` | `#include <math.h> float a = floor(1.7); float b = ceil(1.2);` | `ÔÇö` |
| `valido_math_valores` | `#include <math.h>  printf("sqrt(16)=%f\n", sqrt(16.0)); printf("pow(2,3)=%f\n...` | `sqrt(16)=4.000000 pow(2,3)=8.000000 abs(-5)=5.000000 floor(3.14)=3.000000 ceil(3.14)=4.000000 round(3.5)=4.000000 floor(-2.5)=-3.000000 ceil(-2.5)=-2.000000 pow(2,-1)=0.500000` |
| `valido_pow_literal` | `#include <math.h> float x = pow(2.0, 3.0);` | `ÔÇö` |
| `valido_round_literal` | `#include <math.h> float x = round(2.5);` | `ÔÇö` |
| `valido_sqrt_literal` | `#include <math.h> float x = sqrt(16.0);` | `ÔÇö` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_math_funcao_nao_suportada` | `#include <math.h>  float x = log(10.0);` |
| `invalido_math_sqrt_argumentos_extras` | `#include <math.h>  float x = sqrt(16.0, 4.0);` |

---

## Operadores Aritmeticos

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_div_inteira` | `int resto = 10 / 3;` |
| `valido_exp_float` | `float calc = 10.0 / 4.0 + 2.5;` |
| `valido_precedencia` | `int res = (10 + 5) * 2;` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_dois_operadores` | `int x = 10 + * 5;` |
| `invalido_operador_duplo` | `int x = 10 + * 5;` |
| `invalido_parenteses` | `int y = (10 + 5;` |
| `invalido_parenteses_aberto` | `int y = (10 + 5;` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_div_inteira` | `int x = 10; int y = 2; int z = x / y;` |
| `valido_soma_mista` | `int a = 10; float b = 5.5; float c = a + b;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_divisao_zero` | `int x = 1 / "0";` |
| `invalido_mult_string` | `int a = 10; string b = "20"; int c = a * b;` |
| `invalido_soma_string_int` | `string s = "oi"; int x = 10; int res = s + x;` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_atrib_op_fold` | `int x = 1; x = x + 2 + 3;` | `ÔÇö` |
| `valido_div_inteira_print` | `printf("%d", 10 / 3);` | `3` |
| `valido_fold_div_soma` | `int x = 10 / 2 + 1;` | `ÔÇö` |
| `valido_fold_float` | `float f = 1.5 + 2.5;` | `ÔÇö` |
| `valido_fold_precedencia` | `int x = 2 + 3 * 4;` | `ÔÇö` |
| `valido_fold_sub` | `int x = 10 - 3 - 2;` | `ÔÇö` |
| `valido_mix_float_print` | `printf("%f", 1 + 2.5);` | `3.500000` |
| `valido_mod_expr` | `printf("%d", 10 - 3 * 2);` | `4` |
| `valido_precedencia_print` | `printf("%d", 2 + 3 * 4);` | `14` |
| `valido_unary_minus` | `printf("%d", -5 + 10);` | `5` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_div_zero` | `printf("%d", 1 / 0);` |
| `invalido_mult_string` | `int x = "a" * 2;` |
| `invalido_parenteses` | `int x = (1 + 2;` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_soma_string` | `printf("%d", "a" + 1);` |

---

## Operadores Logicos

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_op_logico_and` | `if (a && b) { }` |
| `valido_op_logico_exp_composta` | `if (a && b \|\| c && !d) { }` |
| `valido_op_logico_not` | `if (!flag) { }` |
| `valido_op_logico_or` | `if (x \|\| y) { }` |
| `valido_op_logico_precedencia` | `while (a && (b \|\| c)) { }` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_op_logico_not_sem_operando` | `if (! ) { }` |
| `invalido_op_logico_sem_operando_direito` | `if (a && ) { }` |
| `invalido_op_logico_sem_operando_esquerdo` | `if ( && b) { }` |
| `invalido_op_logico_sem_operando_exp_composta` | `if (a && b \|\| ) { }` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_op_logico_comparacao_char_int` | `char c = 'A'; if (!c) { }` |
| `valido_op_logico_condicao` | `int a = 1, b = 0; if (a && b) { }` |
| `valido_op_logico_condicao_exp_composta_com_constante` | `int flag = 1; if (flag && 1) { }` |
| `valido_op_logico_exp_relacionais` | `float x = 0.0, y = 5.5; if (x \|\| y) { }` |
| `valido_op_not_duplo` | `int a = 5; if (!!a) { }` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_op_logico_condicao_exp_composta` | `string s = "texto"; if (s && 1) { }` |
| `invalido_op_logico_exp_relacionais` | `if (1 < "2") { }` |
| `invalido_op_logico_exp_relacionais_string_literal` | `if ("abc" \|\| 1) { }` |
| `invalido_op_logico_exp_relacionais_struct` | `if ("a" < 1) { }` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_and_curto_circuito` | `if (0 && 1) { int x = 1; }` | `ÔÇö` |
| `valido_and_fold` | `if (1 && 1) { }` | `ÔÇö` |
| `valido_and_print` | `int a = 1, b = 0; printf("%d", a && b);` | `0` |
| `valido_composto_print` | `int a = 1, b = 2; printf("%d", a < b && b > 0);` | `1` |
| `valido_not_fold` | `if (!0) { }` | `ÔÇö` |
| `valido_not_print` | `int a = 0; printf("%d", !a);` | `1` |
| `valido_or_curto_circuito` | `if (1 \|\| 0) { }` | `ÔÇö` |
| `valido_or_print` | `int a = 0, b = 1; printf("%d", a \|\| b);` | `1` |
| `valido_relacional_fold` | `if (1 < 2) { }` | `ÔÇö` |
| `valido_relacional_print` | `printf("%d", 3 < 5);` | `1` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_and_sem_operando` | `int x = && 1;` |
| `invalido_if_string` | `if ("a") printf("x");` |
| `invalido_not_sem_operando` | `int x = !;` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_relacional_string` | `int x = "a" < 1;` |

---

## Otimizador

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_decl_minima` | `int x = 1;` |
| `valido_for_minimo` | `for (;;) { break; }` |
| `valido_if_minimo` | `if (1) { }` |
| `valido_switch_minimo` | `switch (1) { case 1: break; }` |
| `valido_while_minimo` | `while (0) { }` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_chave_aberta` | `int x = 1; {` |
| `invalido_decl_sem_expr` | `int x = ;` |
| `invalido_if_sem_paren` | `if 1 { }` |
| `invalido_ponto_virgula` | `int x = 1` |
| `invalido_while_sem_paren` | `while 1 { }` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_arit_ok` | `int x = 1 + 2;` |
| `valido_atrib_ok` | `int x = 1; x = 2;` |
| `valido_for_ok` | `for (int i = 0; i < 0; i++) { }` |
| `valido_if_zero` | `int x = 1; if (0) { x = 2; }` |
| `valido_while_zero` | `int x = 0; while (0) x = 1;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_div_zero` | `int x = 1 / "0";` |
| `invalido_if_string` | `if ("a") { }` |
| `invalido_redeclaracao` | `int x = 1; int x = 2;` |
| `invalido_tipo_incompativel` | `int x = "a";` |
| `invalido_var_nao_decl` | `x = 1;` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_atrib_op_fold` | `int x = 1; x += 2 + 3; printf("%d\n", x);` | `6` |
| `valido_atrib_op_fold_exec` | `int x = 0; x += 2 + 3; printf("%d", x);` | `5` |
| `valido_atrib_op_noop` | `int x = 5; x += 0; x -= 0; x *= 1; x /= 1; printf("%d\n", x);` | `5` |
| `valido_dce_if` | `int x = 1; if (0) {     x = 99; } else {     x = 2; } printf("%d\n", x);` | `2` |
| `valido_dce_if_exec` | `int x = 1; if (0) { x = 99; } else { x = 2; } printf("%d\n", x);` | `2` |
| `valido_dce_while` | `int x = 0; while (0) x = 1; printf("%d\n", x);` | `0` |
| `valido_dce_while_exec` | `int x = 0; while (0) { x = 1; } printf("%d", x);` | `0` |
| `valido_dce_while_fold` | `int x = 0; while (1 - 1) x = 1; printf("%d\n", x);` | `0` |
| `valido_fold_arit_exec` | `int x = 2 + 3 * 4; printf("%d", x);` | `14` |
| `valido_fold_aritmetico` | `int x = 2 + 3 * 4; printf("%d\n", x);` | `14` |
| `valido_inc_dec` | `int x = 10; x++; x--; x++; printf("%d\n", x);` | `11` |
| `valido_inc_dec_exec` | `int i = 0; i++; i--; printf("%d", i);` | `0` |
| `valido_pos_return` | `int g() {     return 42;     printf("inalcanceavel\n"); }  int r = g(); print...` | `42` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_atrib_literal` | `5 = 1;` |
| `invalido_div_zero` | `printf("%d", 1 / 0);` |
| `invalido_if_string` | `if ("a") printf("x");` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_var_nao_decl` | `printf("%d", z);` |

---

## Recursao

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_fatorial_decl` | `int fat(int n) { return 1; }` |
| `valido_fib_decl` | `int fib(int n) { return 0; }` |
| `valido_func_multi_params` | `int g(int a, int b, int c) { return a; }` |
| `valido_func_recursiva` | `int f(int n) { return n; }` |
| `valido_mdc_decl` | `int mdc(int a, int b) { return a; }` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_chave_aberta` | `int f(int n) { return n;` |
| `invalido_func_paren_aberto` | `int f(int n return n; }` |
| `invalido_func_sem_fechar` | `int f(int n { return n; }` |
| `invalido_param_sem_tipo` | `int f(n) { return n; }` |
| `invalido_return_sem_expr` | `int f(int n) { return; }` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_chamada_mutua_decl` | `int g(int n) { return n + 1; } int f(int n) { return g(n); }` |
| `valido_fatorial_sem` | `int fat(int n) { if (n <= 1) return 1; return n * fat(n - 1); }` |
| `valido_fib_sem` | `int fib(int n) { if (n <= 1) return n; return fib(n-1) + fib(n-2); }` |
| `valido_mdc_sem` | `int mdc(int a, int b) { if (b == 0) return a; return mdc(b, a % b); }` |
| `valido_sombreamento_param` | `int f(int x) { int y = x; return y; }` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_escopo_local` | `// invalido_escopo_local.txt // ERRO ESPERADO: [Semantico] Erro: variavel 'y' nao decla...` |
| `invalido_recursao_argumentos` | `// invalido_recursao_argumentos.txt // ERRO ESPERADO: Erro no Runtime, "espera 1 argume...` |
| `invalido_recursao_infinita` | `// invalido_recursao_infinita.txt // ERRO ESPERADO: Falta de caso base gera um loop inf...` |
| `invalido_recursao_nome` | `// invalido_recursao_nome.txt // ERRO ESPERADO: Erro no Sem├óntico (tempo de checagem), ...` |

### Execucao ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_fold_retorno` | `int f(int n) { return 1 + 2; }` |
| `valido_if_zero_chamada` | `int f(int n) { if (0) f(n); return 1; }` |
| `valido_pos_return_morto` | `int f(int n) { return n; f(n); }` |
| `valido_ramo_morto_recursao` | `int f(int n) { if (0) return f(n); return n; }` |
| `valido_recursao_fatorial` | `// 01_fatorial.txt // Calcula o fatorial de um n├║mero N de forma recursiva  int fat(int...` |
| `valido_recursao_fibonacci` | `// 02_fibonacci.txt // Calcula o N-├®simo n├║mero da sequ├¬ncia de Fibonacci  int fib(int ...` |
| `valido_recursao_mdc` | `// 03_mdc.txt // Calcula o M├íximo Divisor Comum (MDC) usando o algoritmo de Euclides  i...` |
| `valido_sombreamento` | `// valido_sombreamento.txt // Testa sombreamento (shadowing) de variaveis globais e iso...` |
| `valido_while_zero_rec` | `int f(int n) { while (0) f(n); return n; }` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_args_errados` | `int f(int n) { return f(n, n); } int main() { return 0; }` |
| `invalido_chamada_inexistente` | `int main() { return foo(1); }` |
| `invalido_escopo` | `int f(int n) { return x; } int main() { return 0; }` |
| `invalido_nome_errado` | `int f(int n) { return g(n); } int main() { return 0; }` |
| `invalido_printf_args` | `printf("%d %d", 1);` |

---

## Stdlib

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_include_stdlib` | `#include <stdlib.h>` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_atoi_aberto` | `#include <stdlib.h> int x = atoi("42";` |
| `invalido_exit_aberto` | `#include <stdlib.h> exit(0` |
| `invalido_include_aberto` | `#include <stdlib` |
| `invalido_include_malformado` | `#include <stdlib.h` |
| `invalido_include_token` | `#include stdlib.h>` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_stdlib_todas_funcoes` | `#include <stdlib.h>  int a = atoi("-17"); float b = atof("2.5");` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_stdlib_exit_sem_argumento` | `#include <stdlib.h>  exit();` |
| `invalido_stdlib_funcao_sem_include` | `int a = atoi("10");` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada | Exit code |
|------|---------|----------------|-----------|
| `valido_atof_fold` | `#include <stdlib.h> float x = atof("1.0") + 0.0;` | `ÔÇö` | `ÔÇö` |
| `valido_atof_literal` | `#include <stdlib.h> float x = atof("3.14");` | `ÔÇö` | `ÔÇö` |
| `valido_atoi_expr` | `#include <stdlib.h> int x = atoi("10") + 0;` | `ÔÇö` | `ÔÇö` |
| `valido_atoi_literal` | `#include <stdlib.h> int x = atoi("42");` | `ÔÇö` | `ÔÇö` |
| `valido_exit_fold` | `#include <stdlib.h> if (0) exit(1 + 2);` | `ÔÇö` | `ÔÇö` |
| `valido_stdlib_exit_0` | `#include <stdlib.h>  exit(0);` | `ÔÇö` | `0` |
| `valido_stdlib_exit_1` | `#include <stdlib.h>  exit(1);` | `ÔÇö` | `1` |
| `valido_stdlib_valores` | `#include <stdlib.h>  printf("%d\n", atoi("-17")); printf("%f\n", atof("2.5"))...` | `-17 2.500000 100` | `ÔÇö` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_stdlib_atoi_tipo_errado` | `#include <stdlib.h>  int x = atoi(123);` |
| `invalido_stdlib_funcao_nao_suportada` | `#include <stdlib.h>  int x = rand();` |

---

## Switch/Case

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_switch_case_vazio` | `switch (x) { case 1: }` |
| `valido_switch_char` | `switch (c) { case 'a': break; }` |
| `valido_switch_default` | `switch (x) { default: break; }` |
| `valido_switch_default_e_cases` | `switch (x) { case 1: break; default: break; }` |
| `valido_switch_int` | `switch (x) { case 1: break; }` |
| `valido_switch_multiplos_cases` | `switch (x) { case 1: break; case 2: break; }` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_case_sem_dois_pontos` | `switch (x) { case 1 break; }` |
| `invalido_case_sem_valor` | `switch (x) { case : break; }` |
| `invalido_default_sem_dois_pontos` | `switch (x) { default break; }` |
| `invalido_switch_sem_chaves` | `switch (x) case 1: break;` |
| `invalido_switch_sem_expr` | `switch () { case 1: break; }` |
| `invalido_switch_sem_parenteses` | `switch x { case 1: break; }` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_switch_cases_distintos` | `int x = 1; switch (x) {     case 1: break;     case 2: break;     default: break; }` |
| `valido_switch_char` | `char c = 'a'; switch (c) {     case 'a': break;     case 'b': break; }` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_break_fora_switch` | `break;` |
| `invalido_case_duplicado` | `int x = 1; switch (x) {     case 1: break;     case 1: break; }` |
| `invalido_default_duplo` | `int x = 0; switch (x) {     default: break;     default: break; }` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_dce_after_break` | `int x = 2;     int y;     switch (x) {     case 1:         break;         y =...` | `ÔÇö` |
| `valido_default` | `int x = 10; int y;  switch (x) {     case 1:         y = 10; 	break;     case...` | `30` |
| `valido_empty_case` | `int x = 1; int y;  switch (x) {     case 1:     case 2:         y = 10;      ...` | `10` |
| `valido_fall-through` | `int x = 10; int y;  switch (x) {     case 10:         y = 10;     case 2:    ...` | `20` |
| `valido_one_case` | `int x = 2; int y = 1;  switch (x) {     case 2: {         y = 2;     } }  pri...` | `2` |
| `valido_only_default` | `int x; int y;  switch (x) {     default:         y = 2; }  printf("%d", y);` | `2` |
| `valido_switch_break` | `int x = 1; int r = 0; switch (x) {     case 1: r = 1; break;     case 2: r = ...` | `3` |
| `valido_switch_char_exec` | `char c = 'b'; int r = 0; switch (c) {     case 'a': r = 1; break;     case 'b...` | `2` |
| `valido_with_return` | `int f() {     int x = 2;     int y;     switch (x) {     case 1:         y = ...` | `20` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_break_fora` | `break;` |
| `invalido_case_duplicado` | `switch (1) { case 1: break; case 1: break; }` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_sem_match_sem_default` | `switch (1) { case 1: break; case 1: break; }` |
| `invalido_switch_sem_expr` | `switch () { case 1: break; }` |

---

## While

### Sintatico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_while_com_chaves` | `while (x > 0) { x = x - 1; }` |
| `valido_while_condicao_com_parenteses_extras` | `while ((a+b) < c) { }` |
| `valido_while_corpo_vazio` | `while (x > 0) ;` |
| `valido_while_pos_incremento` | `while (i < 10) i++;` |
| `valido_while_sem_chaves` | `while (1) a = a + 1;` |

### Sintatico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_while_com_chaves_instrucao` | `while (x < 10) { x = x + 1 }` |
| `invalido_while_condicao_sem_fechar_parenteses` | `while (x < 10 { }` |
| `invalido_while_condicao_sem_parenteses` | `while x < 10 { }` |
| `invalido_while_sem_chaves_instrucao` | `while (x < 10) x = x + 1` |
| `invalido_while_sem_fechar_chaves` | `while (x < 10) {` |

### Semantico ÔÇö Validos

| Caso | Entrada |
|------|---------|
| `valido_while_condicao_exp_relacional_menor` | `int i = 0; while (i < 10) i++;` |
| `valido_while_condicao_exp_relacional_menor_igual` | `int a = 1; while (a <= 10) a = a * 2;` |
| `valido_while_condicao_pos_incremento` | `int x = 0; while (x++ < 5) ;` |
| `valido_while_conversao_float_condicao` | `float f = 5.5; while (f) f = f - 1.0;` |
| `valido_while_conversao_int_condicao` | `int flag = 1; while (flag) flag = 0;` |

### Semantico ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_while_condicao_exp_relacional` | `while (1 < "2") { }` |
| `invalido_while_condicao_null` | `while (NULL) x--;` |
| `invalido_while_condicao_var_nao_declarada` | `while (x < 10) x = x + 1;` |
| `invalido_while_condicao_void` | `int y = 1; void a; while (a) {     y++; }` |
| `invalido_while_conversao_string_condicao` | `int x; while ("texto") x = 1;` |

### Execucao ÔÇö Validos

| Caso | Entrada | Saida esperada |
|------|---------|----------------|
| `valido_while_break` | `int i = 0; while (1) { i++; break; } printf("%d", i);` | `1` |
| `valido_while_break_dce` | `int x = 0; while (1) { break; x = 5; }` | `ÔÇö` |
| `valido_while_cond_fold` | `int x = 0; while (1 - 1) x = 1;` | `ÔÇö` |
| `valido_while_contador` | `int i = 0; while (i < 3) i++; printf("%d", i);` | `3` |
| `valido_while_corpo_morto` | `int x = 0; while (0) { x = 1; x = 2; }` | `ÔÇö` |
| `valido_while_corpo_vazio_falso` | `int x = 0; while (0) x = 1; printf("%d", x);` | `0` |
| `valido_while_float_cond` | `float f = 1.0; while (f) { f = 0; } printf("%f", f);` | `0.000000` |
| `valido_while_literal_fold` | `int x = 0; while (2 - 2) x = 1;` | `ÔÇö` |
| `valido_while_soma` | `int i = 0, s = 0; while (i < 3) { s += i; i++; } printf("%d", s);` | `3` |
| `valido_while_zero` | `int x = 0; while (0) x = 1;` | `ÔÇö` |

### Execucao ÔÇö Invalidos

| Caso | Entrada |
|------|---------|
| `invalido_div_zero_corpo` | `int x = 1 / 0; printf("%d", x);` |
| `invalido_printf_args` | `printf("%d %d", 1);` |
| `invalido_var_nao_decl` | `while (x) { }` |
| `invalido_while_string` | `while ("a") { }` |
| `invalido_while_void` | `while (void) { }` |

---

## Hist├│rico de Vers├úo

| Vers├úo | Data | Descri├º├úo | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 13/05/26 | Cria├º├úo da p├ígina com seu respectivo conte├║do | Camila Careli |
| 2.0 | 21/06/26 | Reestrutura├º├úo dos casos de teste | Camila Careli |
