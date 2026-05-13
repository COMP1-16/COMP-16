# Linguagem interpretada

> Especificação técnica de referência para o projeto: tokens, exemplos de sintaxe, semântica ilustrativa e erros típicos. Para o contexto geral sobre a linguagem C e a relação com o escopo do interpretador, veja [Linguagem e Escopo — visão geral](linguagem-e-escopo.md).


## Sintaxe e Semântica da Linguagem C  

Toda linguagem possui duas dimensões fundamentais: 

- **Sintaxe**: regras de escrita correta;
- **Semântica**: significado do código.


### 1. Sintaxe  

#### 1.1 Tokens: Os tokens são os elementos básicos da linguagem:

##### Palavras-chave
```c
int, float, double, char, void, if, else, while, for, do,
return, struct, union, typedef, enum, switch, case, break,
continue, goto, sizeof, static, extern, const, volatile
```

##### Identificadores
```c
int contador;
float temperatura_media;
char _buffer[256];
```

##### Literais
```c
42
3.14
'A'
"linguagem"
```

##### Operadores
```c
+  -  *  /  %
== != < > <= >=
&& || !
& | ^ ~ << >>
= += -= *= /=
```

##### Delimitadores
```c
;
,
{}
()
[]
```


#### 1.2 Estrutura de um programa
```c
#include <stdio.h>

int main(void) {
    printf("Olá, mundo!\n");
    return 0;
}
```

#### 1.3 Declarações
```c
int idade;
float salario = 3500.00;
char inicial = 'M';
double pi = 3.141592653589793;
```

##### Protótipos:
```c
int somar(int a, int b);
float calcular_media(float notas[], int quantidade);
```

#### 1.4 Instruções e blocos
```c
x = 10;

{
    int temp;
    temp = a;
    a = b;
    b = temp;
}
```

#### 1.5 Estruturas de controle  

##### Condicional
```c
if (expressão) {
    // instruções
} else {
    // instruções alternativas
}
```

##### While
```c
while (expressão) {
    // instruções
}
```

##### Do-while
```c
do {
    // instruções
} while (expressão);
```

##### For
```c
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
}
```

##### Switch
```c
switch (expressão) {
    case valor1:
        break;
    case valor2:
        break;
    default:
}
```

#### 1.6 Erros sintáticos
```c
int x = 10        // falta ;
if x > 0 {        // falta ()
int = 5;          // identificador ausente
float 2y = 3.0;   // inválido
```

---

### 2. Semântica  

#### 2.1 Tipos
```c
int a = 5, b = 2;
int r1 = a / b;
double r2 = (double)a / b;
```

#### 2.2 Operadores
```c
int x = 5;

if (x = 10) { }
if (x == 10) { }
```

##### Incremento:
```c
int a = 5;
int b = a++;
int c = ++a;
```

#### 2.3 Ponteiros
```c
int x = 42;
int *p = &x;

*p = 100;
```

##### Array:
```c
int v[5] = {10, 20, 30, 40, 50};
int *p = v;

*(p+1);
```

#### 2.4 Escopo
```c
int global = 1;

void funcao(void) {
    int local = 2;
    static int s = 0;
}
```

#### 2.5 Funções

##### Passagem por valor:
```c
void dobrar(int n) {
    n = n * 2;
}
```

##### Por referência:
```c
void dobrar(int *n) {
    *n = *n * 2;
}
```

#### 2.6 Comportamento indefinido
```c
int v[5];
v[10] = 99;

int x;
printf("%d\n", x);

int max = 2147483647;
int r = max + 1;

int *p = NULL;
*p = 5;
```

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 18/04/26 | Criação da página inicial | Pedro Henrique |
| 1.1 | 13/05/26 | Revisão do conteúdo e da estética da página | Camila Careli |