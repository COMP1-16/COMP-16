# Ponto de Controle 1 (PC1)

> Marco correspondente às entregas concluídas até o final da **Sprint 2** (28/04 – 11/05/2026).

## Resumo das entregas

No PC1, a equipe estabeleceu a base do interpretador e da documentação do projeto:

### Documentação e organização

- Definição do escopo da linguagem C a ser interpretada ([#2](https://github.com/COMP1-16/COMP-16/issues/2)).
- Documento de sintaxe e semântica básicas da linguagem C ([#1](https://github.com/COMP1-16/COMP-16/issues/1)).
- Configuração inicial do repositório a partir do template da disciplina ([#3](https://github.com/COMP1-16/COMP-16/issues/3)).
- Inclusão do `Makefile` e instruções de build/teste no README ([#18](https://github.com/COMP1-16/COMP-16/issues/18)).

### Análise léxica e sintática

- Regras léxicas iniciais no Flex (`lexer.l`): palavras-chave, tipos, identificadores, literais e operadores ([#4](https://github.com/COMP1-16/COMP-16/issues/4)).
- Gramática Bison inicial (`parser.y`): literais, declarações de variáveis, atribuições e operações aritméticas ([#6](https://github.com/COMP1-16/COMP-16/issues/6)).
- Casos de teste para a gramática inicial ([#8](https://github.com/COMP1-16/COMP-16/issues/8)).

### Estrutura de execução

- Estrutura da AST e funções de criação de nós ([#15](https://github.com/COMP1-16/COMP-16/issues/15)).
- Tabela de símbolos para variáveis e escopos ([#17](https://github.com/COMP1-16/COMP-16/issues/17)).
- Percorrimento da AST para execução ([#16](https://github.com/COMP1-16/COMP-16/issues/16)).
- Interpretação do escopo inicial: literais, declarações, atribuições e operadores aritméticos ([#14](https://github.com/COMP1-16/COMP-16/issues/14)).

### Pesquisa e planejamento

- Levantamento de testes automatizados ([#10](https://github.com/COMP1-16/COMP-16/issues/10)).
- Mapeamento de lacunas para a interpretação do escopo inicial ([#11](https://github.com/COMP1-16/COMP-16/issues/11)).

## Artefatos no repositório

| Componente | Localização |
|------------|-------------|
| Lexer | `lexer/lexer.l` |
| Parser | `parser/parser.y` |
| AST | `lib/ast/` |
| Tabela de símbolos | `lib/simbols/` |
| Build | `Makefile` |

---

## Histórico de Versão

| Versão | Data | Descrição | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas até a Sprint 2 (PC1) | Pedro Henrique |
