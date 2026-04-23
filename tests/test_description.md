# Casos de teste

## Code
### Válidos:
- codeblock
- codeblock codeblock
- codeblock codeblock KW_RETURN expressao SEMICOLON

Exemplo:
- TYPE_INT ID SEMICOLON
- TYPE_INT ID SEMICOLON TYPE_INT ID SEMICOLON
- TYPE_INT ID SEMICOLON TYPE_INT ID SEMICOLON KW_RETURN ID SEMICOLON

### Inválidos
- 

## Codeblock
### Notas:
- Bloco de função não implementado;
- Controle de fluxo não implementado;
- Loop não implementado;
### Válidos:
- ID PLUS NUM SEMICOLON
- TYPE_INT ID SEMICOLON
- Acomoda atualização;

## Declaracao:
### Válidos:
- TYPE_INT(tipo) ID SEMICOLON
- TYPE_INT ID(1) INITVAR NUM(1) SEMICOLON
- TYPE_INT(tipo) ID(1) COMMA ID(2) SEMICOLON
- TYPE_INT ID(1) INITVAR NUM(1) COMMA ID(2) INITVAR NUM(2) SEMICOLON



