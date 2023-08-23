#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 4. Escrever um programa para determinar o MDC e o MMC de dois inteiros com base em seus fatores primos.

int algorithm(bool isMin, int length, int decomposed[], int decomposed_exponents[])
{
    int result = 1;

    for (int i = 0; i < length; i++)
    {
        if (i != 0 && i != length)
        {
            printf(" *");
        }

        int exponent = 0;
        for (int check = 0; check < length; check++)
        {
            // printf("\ni original: %d | check: %d\n", i, check);
            //  Caso encontremos outro número igual na array, verificamos se o expoente dele é menor, em MDC's, ou maior, em MMC's, e o substituímos pelo expoente padrão 0;

            if (isMin && check == i)
            {
                break;
            }

            if (decomposed[check] == decomposed[i])
            {
                exponent = isMin ? fmin(decomposed_exponents[check], decomposed_exponents[i]) : fmax(decomposed_exponents[check], decomposed_exponents[i]);
            }
        }

        printf(" %d^%d", decomposed[i], exponent);
        result *= (pow(decomposed[i], exponent));
    }

    printf(" = %d\n", result);
    return result;
}

int main()
{

    int input1, input2;

    printf("🔢 >> Insira dois números inteiros para determinar seus MDC e MMC: ");
    int scan_return = scanf("%d %d", &input1, &input2);

    if (scan_return != 2)
    {
        printf("❌ Os caracteres digitados formam uma entrada inválida!\n");
        return 0;
    }

    struct timespec start_time = start_clock();
    printf("Estes são os MDC e MMC dos dois números informados: ⬇️\n");

    int decomposed1[10000] = {0};
    int decomposed2[10000] = {0};

    int decomposed1_exponents[10000] = {0};
    int decomposed2_exponents[10000] = {0};

    int length1 = decompose(input1, decomposed1, decomposed1_exponents);
    int length2 = decompose(input2, decomposed2, decomposed2_exponents);

    /* printf("Decomposed 1:\n");
    for (int i = 0; i < length1; i++)
    {
        printf("%d - ", decomposed1[i]);
    }

    printf("\nEXPOENTS 1:\n");
    for (int i = 0; i < length1; i++)
    {
        printf("%d - ", decomposed1_exponents[i]);
    }

    printf("\nDecomposed 2:\n");
    for (int i = 0; i < length2; i++)
    {
        printf("%d - ", decomposed2[i]);
    }

    printf("\nEXPOENTS 2:\n");
    for (int i = 0; i < length2; i++)
    {
        printf("%d - ", decomposed2_exponents[i]);
    } */

    int decomposed[10000] = {0};
    int decomposed_exponents[100000] = {0};
    int length = length1 + length2;

    for (int i = 0; i < length; i++)
    {
        if (i < length1)
        {
            decomposed[i] = decomposed1[i];
            decomposed_exponents[i] = decomposed1_exponents[i];
        }
        else
        {
            decomposed[i] = decomposed2[i - length1];
            decomposed_exponents[i] = decomposed2_exponents[i - length1];
        }
    }

    /* for (int i = 0; i < length; i++)
    {
        printf("Decomposed: %d e Exponent: %d\n", decomposed[i], decomposed_exponents[i]);
    } */

    // MDC
    printf("mdc(%d, %d) =", input1, input2);
    algorithm(true, length, decomposed, decomposed_exponents);

    // MDC
    printf("mmc(%d, %d) =", input1, input2);
    algorithm(false, length, decomposed, decomposed_exponents);

    end_clock(start_time);

    return 0;
}