#include <stdio.h>
#include <math.h>

#include "./modules/prime.h"
#include "./modules/timer.h"

// 3. Escrever um programa para decompor um determinado número inteiro em seus fatores primos.

int main()
{

    printf("🔢 >> Insira um número inteiro para decompor em seus fatores primos: ");
    int input;
    int scan_return = scanf("%d", &input);

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

    printf("⏳ Decompondo %d em seus fatores primos...\n", input);
    struct timespec start_time = start_clock();

    printf("Estes são os fatores primos extraídos no número inserido: ⬇️\n");

    int decomposed[input];
    int expoents[input];

    int amount = decompose(input, decomposed, expoents);

    for (int i = 0; i < amount; i++)
    {
        printf("- %d^%d -\n", decomposed[i], expoents[i]);
    }

    end_clock(start_time);

    return 0;
}