# Documento Inicial de Especificação da Linguagem  
## Interpretador C - Sprint 1  

**Grupo 16**  
**27 de março de 2026**  

### Descrição Técnica: Tokens, Estruturas e Exemplos de Código  

---

## Sumário

- 1. Visão Geral  
- 2. Introdução  
- 3. Sintaxe e Semântica  
  - 3.1 Sintaxe  
  - 3.2 Semântica  

---

## 1. Visão Geral  
Este documento descreve a especificação inicial da linguagem C. O objetivo desta especificação é guiar o desenvolvimento do interpretador, definindo claramente a sintaxe e semântica básica suportada pelo projeto.  

---

## 2. Introdução  
A linguagem C é uma linguagem de programação de propósito geral, de nível médio, desenvolvida por Dennis Ritchie nos Laboratórios Bell entre os anos de 1969 e 1973.  

Originalmente concebida para o desenvolvimento do sistema operacional UNIX, C tornou-se uma das linguagens mais influentes da história da computação, servindo de base para inúmeras linguagens modernas, como C++, Java, C# e Python.  

Caracterizada por sua eficiência, portabilidade e controle direto sobre o hardware, a linguagem C permanece amplamente utilizada em sistemas embarcados, sistemas operacionais, compiladores e aplicações de alto desempenho.  

---

## 3. Sintaxe e Semântica da Linguagem C  

Toda linguagem possui duas dimensões fundamentais:  
- **Sintaxe**: regras de escrita correta  
- **Semântica**: significado do código  

---

## 3.1 Sintaxe  

### 3.1.1 Tokens  

Os tokens são os elementos básicos da linguagem:

#### Palavras-chave
```c
int, float, double, char, void, if, else, while, for, do,
return, struct, union, typedef, enum, switch, case, break,
continue, goto, sizeof, static, extern, const, volatile
```

#### Identificadores
```c
int contador;
float temperatura_media;
char _buffer[256];
```

#### Literais
```c
42
3.14
'A'
"linguagem"
```

#### Operadores
```c
+  -  *  /  %
== != < > <= >=
&& || !
& | ^ ~ << >>
= += -= *= /=
```

#### Delimitadores
```c
;
,
{}
()
[]
```

---

### 3.1.2 Estrutura de um programa
```c
#include <stdio.h>

int main(void) {
    printf("Olá, mundo!\n");
    return 0;
}
```

---

### 3.1.3 Declarações
```c
int idade;
float salario = 3500.00;
char inicial = 'M';
double pi = 3.141592653589793;
```

Protótipos:
```c
int somar(int a, int b);
float calcular_media(float notas[], int quantidade);
```

---

### 3.1.4 Instruções e blocos
```c
x = 10;

{
    int temp;
    temp = a;
    a = b;
    b = temp;
}
```

---

### 3.1.5 Estruturas de controle  

#### Condicional
```c
if (expressão) {
    // instruções
} else {
    // instruções alternativas
}
```

#### While
```c
while (expressão) {
    // instruções
}
```

#### Do-while
```c
do {
    // instruções
} while (expressão);
```

#### For
```c
for (int i = 0; i < 10; i++) {
    printf("%d\n", i);
}
```

#### Switch
```c
switch (expressão) {
    case valor1:
        break;
    case valor2:
        break;
    default:
}
```

---

### 3.1.6 Erros sintáticos
```c
int x = 10        // falta ;
if x > 0 {        // falta ()
int = 5;          // identificador ausente
float 2y = 3.0;   // inválido
```

---

## 3.2 Semântica  

### 3.2.1 Tipos
```c
int a = 5, b = 2;
int r1 = a / b;
double r2 = (double)a / b;
```

---

### 3.2.2 Operadores
```c
int x = 5;

if (x = 10) { }
if (x == 10) { }
```

Incremento:
```c
int a = 5;
int b = a++;
int c = ++a;
```

---

### 3.2.3 Ponteiros
```c
int x = 42;
int *p = &x;

*p = 100;
```

Array:
```c
int v[5] = {10, 20, 30, 40, 50};
int *p = v;

*(p+1);
```

---

### 3.2.4 Escopo
```c
int global = 1;

void funcao(void) {
    int local = 2;
    static int s = 0;
}
```

---

### 3.2.5 Funções

Passagem por valor:
```c
void dobrar(int n) {
    n = n * 2;
}
```

Por referência:
```c
void dobrar(int *n) {
    *n = *n * 2;
}
```

---

### 3.2.6 Comportamento indefinido
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
