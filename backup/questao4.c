#include <stdio.h>
#include <stdbool.h>

#include "../modules/timer.h"
#include "../modules/prime.h"

// 4. Escrever um programa para determinar o MDC e o MMC de dois inteiros com base em seus fatores primos.

int main()
{
    struct timespec start_time = start_clock();

    unsigned long input1, input2;

    printf("🔢 >> Insira dois números inteiro para determinar seus MDC e MMC: ");
    int scan_return = scanf("%lu %lu", &input1, &input2);

    if (scan_return != 2)
    {
        printf("❌ Os caractere digitados formam uma entrada válida!\n");
        return 0;
    }

    printf("Estes são os MDC e MMC dos dois números informados: ⬇️\n");

    unsigned long decomposed1[input1];
    unsigned long decomposed2[input2];

    int *decomposed1;
    int *decomposed2;

    int amount1 = decompose(input1, decomposed1);
    int amount2 = decompose(input2, decomposed2);

    unsigned long factors1[amount1];

    printf("Decomposto:\n");
    for (unsigned long i = 0; i < amount1; i++)
    {
        printf("%lu\n", decomposed1[i]);
    }

    for (unsigned long i = 0; i < amount1; i++)
    {
    }

    for (unsigned long i = 0; i < amount1; i++)
    {
        printf("%lu. %lu\n", i, factors1[i]);
    }

    end_clock(start_time);

    return 0;
}