#include <stdio.h>
#include <stdbool.h>
// #include <string.h>

#include "./modules/timer.h"
#include "./modules/prime.h"
#include "./modules/input.h"

// 9. Escrever um programa para encontrar a solução única de três congruências usando o Teorema Chinês do Resto.
// (Lembre-se de verificar se os módulos são co-primos).

int theorem(int array_size, int array_b[], int array_m[])
{
    // Verificamos se todos os valores m são coprimos (primos entre si, ou seja, o mdc entre eles resulta em 1)
    // Caso não sejam, não é possível encontrar uma solução
    for (int i = 0; i < array_size - 1; i++)
    {
        if (euclides(array_m[i], array_m[i + 1]) != 1)
        {
            break;
            return 0;
        }
    }

    // Alocamos espaço na memória dos M, M1 e M1inverse que iremos calcular
    int array_M[array_size];
    // memset(array_M, 1, array_size);
    int array_M1[array_size];
    int array_M1inverse[array_size];

    // Calculamos os n elementos M
    // O valor de M equivale à multiplicação dos m dos outros índices (i)
    // Exemplo: se m1 = 3, m2 = 7 e m3 = 10, então M1 = 7 * 10 = 70, M2 = 3 * 10 = 30 e M3 = 3 * 7 = 21
    int generalM = 1;
    for (int i = 0; i < array_size; i++)
    {
        generalM *= array_m[i];
    }

    for (int i = 0; i < array_size; i++)
    {
        array_M[i] = generalM / array_m[i];
        // printf("array_M[%d]: %d\n", i, array_M[i]);
    }

    // Calculamos os n elementos M1
    for (int i = 0; i < array_size; i++)
    {
        array_M1[i] = array_M[i] % array_m[i];
    }

    // Calculamos os n elementos M1inverse
    for (int i = 0; i < array_size; i++)
    {
        array_M1inverse[i] = mod_inverse(array_M1[i], array_m[i]);
    }

    // Depois de calcularmos todos os M, M1 e M1inverse, podemos realizar o cálculo final
    int total_multiplication = 0;
    int total_divisor = 1;

    for (int i = 0; i < array_size; i++)
    {
        total_multiplication += array_b[i] * array_M[i] * array_M1inverse[i];
        total_divisor *= array_m[i];
    }

    // Retornamos o "x" (solução) comum entre todas as congruências
    return total_multiplication % total_divisor;
}

int main()
{
    struct timespec start_time = start_clock();

    printf("🔢 >> Insira quantas congruências do tipo x ≅ b mod m você deseja calcular: ");

    int array_size = 0;
    int scan_return = scanf("%d", &array_size);

    if (scan_return != 1)
    {
        printf("✖️ O input inserido não satisfaz o padrão: [quantidade de congruências]\n");
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

    int result = theorem(array_size, array_b, array_m);

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