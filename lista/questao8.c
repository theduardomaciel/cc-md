#include <stdio.h>
#include <stdbool.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 8. Escrever um programa para encontrar solução de uma congruência do tipo ax ≅ b mod m.

int main()
{
    struct timespec start_time = start_clock();

    unsigned long long input;

    printf("🔢 >> Insira três inteiros (a, b e m) para encontrar a solução de uma congruência do tipo ax ≅ b mod m: ");
    int scan_return = scanf("%llu", &input);

    if (scan_return != 1)
    {
        printf("❌ O caractere digitado não é um número válido!\n");
        return 0;
    }

    end_clock(start_time);

    return 0;
}