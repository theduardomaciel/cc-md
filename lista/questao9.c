#include <stdio.h>
#include <stdbool.h>
// #include <string.h>

#include "./modules/timer.h"
#include "./modules/prime.h"
#include "./modules/input.h"

// 9. Escrever um programa para encontrar a solução única de três congruências usando o Teorema Chinês do Resto.
// (Lembre-se de verificar se os módulos são co-primos).

int main()
{
    printf("🔢 >> Insira 3 vezes (uma por linha), dois inteiros (b e m) para encontrar a solução de 3 congruências do tipo x ≅ b mod m:\n");

    // Alocamos espaço na memória dos b e m que iremos calcular
    int array_b[3];
    int array_m[3];
    // memset(array_m, 1, array_size);

    // Requerimos o input do usuário para cada uma das congruências
    int success = request_input(3, array_b, array_m);

    if (!success)
    {
        return 0;
    }

    struct timespec start_time = start_clock();

    int result = chinese_theorem(3, array_b, array_m);

    if (result == 0)
    {
        printf("❌ Não foi possível calcular a solução das congruências lineares informadas.\n");
    }
    else
    {
        printf("✅ A solução das 3 congruências lineares informadas é: %d\n", result);
    }

    end_clock(start_time);

    return 0;
}