#include <stdio.h>
#include <stdbool.h>
// #include <string.h>

#include "./modules/timer.h"
#include "./modules/prime.h"
#include "./modules/input.h"

// 10. Generalize o programa da questão anterior para funcionar com n congruências ao invés de somente 3, onde n será dado pelo usuário.

int main()
{
    printf("🔢 >> Insira quantas congruências do tipo x ≅ b mod m você deseja calcular: ");

    int array_size = 0;
    int scan_return = scanf("%d", &array_size);

    if (scan_return != 1 || array_size == 0)
    {
        printf("✖️ O input inserido não satisfaz o padrão: [quantidade de congruências diferente de 0]\n");
        return 0;
    }

    printf("📝 >> Insira %d vezes (uma por linha), dois inteiros (b e m) para encontrar a solução de %d congruências do tipo x ≅ b mod m:\n", array_size, array_size);

    // Alocamos espaço na memória dos b e m que iremos calcular
    int array_b[array_size];
    int array_m[array_size];
    // memset(array_m, 1, array_size);

    // Requerimos o input do usuário para cada uma das congruências
    int success = request_input(array_size, array_b, array_m);

    if (!success)
    {
        return 0;
    }

    struct timespec start_time = start_clock();

    int result = chinese_theorem(array_size, array_b, array_m);

    if (result == 0)
    {
        printf("❌ Não foi possível calcular a solução das congruências lineares informadas.\n");
    }
    else
    {
        printf("✅ A solução das %d congruências lineares informadas é: %d\n", array_size, result);
    }

    end_clock(start_time);

    return 0;
}