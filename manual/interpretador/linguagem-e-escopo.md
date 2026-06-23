# Linguagem e Escopo

## Visão geral

Esta parte da documentação reúne tudo que diz respeito à linguagem de entrada do interpretador e ao recorte que o projeto se propõe a implementar.

- Na subpágina **[Linguagem interpretada](linguagemInterpretada.md)** você encontra a especificação técnica de referência: tokens, exemplos de sintaxe e semântica ilustrativa da linguagem C no contexto do trabalho.
- Na subpágina **[Escopo](escopo.md)** está a lista objetiva do que entra no interpretador (tipos, operadores, fluxo de controle, funções e subconjunto de bibliotecas).

Assim separamos o “o que é a linguagem C (conceitualmente)” e o “o que este projeto cobre” da documentação detalhada de sintaxe e semântica.

---

## Sobre a linguagem C

A linguagem C é de propósito geral e nível médio, criada por **Dennis Ritchie** nos Laboratórios Bell, entre 1969 e 1973. Foi pensada em grande medida para o sistema UNIX e tornou-se uma das bases mais influentes da computação: C++, Java, C#, Python e muitas outras linguagens herdaram ideias dela.

C combina eficiência, portabilidade e controle próximo ao hardware, o que explica seu uso em sistemas operacionais, embarcados, drivers e compiladores — inclusive como linguagem usual para implementar tradutores como o deste repositório.

O interpretador da Equipe 16 não pretende cobrir a norma inteira da linguagem; trabalha-se com um subconjunto alinhado à disciplina de Compiladores, descrito na página de Escopo e ilustrado na de Linguagem interpretada.

---

## Sobre o escopo do interpretador

Este projeto implementa um interpretador para um subconjunto da linguagem C, com foco em tipos primitivos, operadores, controle de fluxo e funções. O escopo do interpretador é definido para atender aos objetivos do curso de Compiladores, permitindo a execução de programas escritos nesse subconjunto.

Se inclui no escopo:

- **Tipos primitivos**: `int`, `float`, `double`, `char`, `bool`, `void`, `string`;
- **Operadores aritméticos, relacionais e lógicos**;
- **Controle de fluxo**: `if`, `else`, `while`, `for`, `switch`, `case`, `default`, `break`, `return`;
- **Funções**, incluindo declaração, definição e chamada, com suporte a parâmetros e retorno de valores;
- **Bibliotecas padrão**: `math.h` e `stdlib.h`, com funções como `printf`.

## Referências

- [C (linguagem de programação) — Wikipédia, a enciclopédia livre](https://pt.wikipedia.org/wiki/C_(linguagem_de_programa%C3%A7%C3%A3o))

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 18/04/26 | Criação da página inicial | Pedro Henrique |
| 1.1 | 13/05/26 | Revisão do conteúdo e da estética da página | Camila Careli |