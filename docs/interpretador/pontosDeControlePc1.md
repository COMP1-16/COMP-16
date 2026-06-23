# Ponto de Controle 1 (PC1)

> Marco correspondente ├ás entregas conclu├¡das at├® o final da **Sprint 2** (28/04 ÔÇô 11/05/2026).

## Resumo das entregas

No PC1, a equipe estabeleceu a base do interpretador e da documenta├º├úo do projeto:

### Documenta├º├úo e organiza├º├úo

- Defini├º├úo do escopo da linguagem C a ser interpretada ([#2](https://github.com/COMP1-16/COMP-16/issues/2)).
- Documento de sintaxe e sem├óntica b├ísicas da linguagem C ([#1](https://github.com/COMP1-16/COMP-16/issues/1)).
- Configura├º├úo inicial do reposit├│rio a partir do template da disciplina ([#3](https://github.com/COMP1-16/COMP-16/issues/3)).
- Inclus├úo do `Makefile` e instru├º├Áes de build/teste no README ([#18](https://github.com/COMP1-16/COMP-16/issues/18)).

### An├ílise l├®xica e sint├ítica

- Regras l├®xicas iniciais no Flex (`lexer.l`): palavras-chave, tipos, identificadores, literais e operadores ([#4](https://github.com/COMP1-16/COMP-16/issues/4)).
- Gram├ítica Bison inicial (`parser.y`): literais, declara├º├Áes de vari├íveis, atribui├º├Áes e opera├º├Áes aritm├®ticas ([#6](https://github.com/COMP1-16/COMP-16/issues/6)).
- Casos de teste para a gram├ítica inicial ([#8](https://github.com/COMP1-16/COMP-16/issues/8)).

### Estrutura de execu├º├úo

- Estrutura da AST e fun├º├Áes de cria├º├úo de n├│s ([#15](https://github.com/COMP1-16/COMP-16/issues/15)).
- Percorrimento da AST para execu├º├úo ([#16](https://github.com/COMP1-16/COMP-16/issues/16)).
- Interpreta├º├úo do escopo inicial: literais, declara├º├Áes, atribui├º├Áes e operadores aritm├®ticos ([#14](https://github.com/COMP1-16/COMP-16/issues/14)).

### Pesquisa e planejamento

- Levantamento de testes automatizados ([#10](https://github.com/COMP1-16/COMP-16/issues/10)).
- Mapeamento de lacunas para a interpreta├º├úo do escopo inicial ([#11](https://github.com/COMP1-16/COMP-16/issues/11)).

## Artefatos no reposit├│rio

| Componente | Localiza├º├úo |
|------------|-------------|
| Lexer | `lexer/lexer.l` |
| Parser | `parser/parser.y` |
| AST | `lib/ast/` |
| Tabela de s├¡mbolos | `lib/simbols/` |
| Build | `Makefile` |

---

## Hist├│rico de Vers├úo

| Vers├úo | Data | Descri├º├úo | Autor |
| :--- | :--- | :--- | :--- |
| 1.0 | 21/06/26 | Resumo das entregas at├® a Sprint 2 (PC1) | Pedro Henrique |
