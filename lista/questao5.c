#include <stdio.h>
#include <stdbool.h>

#include "./modules/timer.h"

// 5. Escrever um programa para determinar o MDC de dois números com base no Algoritmo de Euclides.

/*
    ALGORITMO DE EUCLIDES:
    1. Escolha os dois numeros para tirar mdc(A,D)
    2. Divida A por D, se o resto não for zero divida D (divisor) pelo resto até achar o resto 0
    3. Quando achar uma divisão (entre A e D) com resto 0, o "D" (divisor) dessa divisão será o MDC de A e D originais.

    A = D * Q + R (dividendo = divisor * resultado inteiro + resto da divisão)
    - A: o dividendo (número a ser dividido)
    - D: o divisor
    - Q: o quociente (resultado) da divisão inteira de a por d
    - R: o mod/resto da divisão inteira
*/

int main()
{
    int input1, input2;

    printf("🔢 >> Insira dois números inteiros para determinar seus MDC: ");
    int scan_return = scanf("%d %d", &input1, &input2);

    if (scan_return != 2)
    {
        printf("❌ Os caracteres digitados formam uma entrada inválida!\n");
        return 0;
    }

    struct timespec start_time = start_clock();

    printf("mdc(%d, %d) = %d\n", input1, input2, euclides(input1, input2));

    end_clock(start_time);

    return 0;
}