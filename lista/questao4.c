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

    /* int decomposed1[input1];
    int decomposed2[input2];

    int *decomposed1;
    int *decomposed2;

    decompose(input1, decomposed1);
    decompose(input2, decomposed2); */

    end_clock(start_time);

    return 0;
}