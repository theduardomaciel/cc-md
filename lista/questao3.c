#include <stdio.h>
#include <math.h>

#include "../modules/prime.h"
#include "../modules/timer.h"

// 3. Escrever um programa para decompor um determinado número inteiro em seus fatores primos.

int main()
{
    struct timespec start_time = start_clock();

    printf("🔢 >> Insira um número inteiro para decompor em seus fatores primos: ");
    unsigned long input;
    int scan_return = scanf("%lu", &input);

    if (scan_return != 1)
    {
        printf("❌ O caractere digitado não é um número válido!\n");
        return 0;
    }

    if (input < 2)
    {
        printf("❌ Este número não possui fatores primos!\n");
        return 0;
    }

    printf("⏳ Decompondo %lu em seus fatores primos...\n", input);

    printf("Estes são os fatores primos extraídos no número inserido: ⬇️\n");

    decompose(input);
    /* unsigned long decomposed[input];
    int amount = decompose(input, decomposed);

    for (unsigned long i = 0; i < amount; i++)
    {
        printf("- %lu -\n", decomposed[i]);
    } */

    end_clock(start_time);

    return 0;
}