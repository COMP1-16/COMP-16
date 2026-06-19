#include "math.h"

// Aqui utilizamos métodos numéricos para implementar funções matemáticas de forma computacional.

// Calcula a raiz quadrada usando o método de Newton-Raphson. 
float math_sqrt(float x) {
    if (x <= 0.0f) return 0.0f; // Retorna 0 para números negativos ou zero

    float palpite = x / 2.0f;
    float tolerancia = 0.00001f;
    
    while (1) {
        float proximo_palpite = (palpite + x / palpite) / 2.0f;
        float diferenca = proximo_palpite - palpite;
        
        // Módulo manual: garante que a diferença seja sempre positiva
        if (diferenca < 0) {
            diferenca = -diferenca; 
        }
        
        // Condição de parada: o erro ficou menor que a tolerância
        if (diferenca < tolerancia) {
            return proximo_palpite;
        }
        
        palpite = proximo_palpite;
    }
}

// Calcula a potência usando repetição simples para expoentes inteiros (convertidos a partir de float).
float math_pow(float base, float expoente) {
    int expoente_inteiro = (int)expoente;
    
    if (expoente_inteiro == 0) return 1.0f;
    if (base == 0.0f) return 0.0f;
    
    float resultado = 1.0f;
    
    // Transforma o expoente em positivo para o laço de repetição
    int expoente_positivo = (expoente_inteiro < 0) ? -expoente_inteiro : expoente_inteiro;
    
    for (int i = 0; i < expoente_positivo; i++) {
        resultado *= base;
    }
    
    // Se o expoente original era negativo, retorna o inverso do resultado
    if (expoente_inteiro < 0) {
        return 1.0f / resultado;
    }
    
    return resultado;
}

// Calcula o valor absoluto (distância até o zero)
float math_abs(float x) {
    if (x < 0.0f) {
        return -x;
    }
    return x;
}

// Arredonda para baixo (para o menor inteiro mais próximo)
float math_floor(float x) {
    int parte_inteira = (int)x;
    //(int)-2.5 vira -2. Mas o floor de -2.5 deve ser -3.
    if (x < 0.0f && x != (float)parte_inteira) {
        return (float)(parte_inteira - 1);
    }
    
    return (float)parte_inteira;
}

// Arredonda para cima (para o maior inteiro mais próximo)
float math_ceil(float x) {
    int parte_inteira = (int)x;
    //int)2.1 vira 2. Mas o ceil de 2.1 deve ser 3.
    if (x > 0.0f && x != (float)parte_inteira) {
        return (float)(parte_inteira + 1);
    }

    return (float)parte_inteira;
}

// Arredonda para o inteiro mais próximo (arredondamento matemático padrão)
float math_round(float x) {
    // Para arredondar corretamente, somamos ou subtraímos 0.5 
    // e reaproveitamos as lógicas de teto e piso que acabamos de criar.
    if (x >= 0.0f) {
        return math_floor(x + 0.5f);
    } else {
        return math_ceil(x - 0.5f);
    }
}