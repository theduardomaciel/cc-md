#include <stdio.h>
#include <stdbool.h>
// #include <string.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 9. Escrever um programa para encontrar a solução única de três congruências usando o Teorema Chinês do Resto.
// (Lembre-se de verificar se os módulos são co-primos).

int main()
{
    struct timespec start_time = start_clock();

    printf("🔢 >> Insira indeterminadas vezes (uma por linha), dois inteiros (b e m) para encontrar a solução de três congruências do tipo x ≅ b mod m.\n");
    printf("Para parar de inserir resultados e imprimir a solução, digite: s\n");

    int result = theorem(3);

    if (result == 0)
    {
        printf("❌ Não foi possível calcular a solução das congruências lineares informadas.\n");
    }
    else
    {
        printf("✅ A solução das congruências lineares informadas é: %d\n", result);
    }

    end_clock(start_time);

    return 0;
}