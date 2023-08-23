#include <stdio.h>
#include <stdbool.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 6. Escrever um programa para encontrar os coeficientes s e t da combinação linear mdc(a,b) = s * a + t * b.

int main()
{
    int A, D;

    printf("🔢 >> Insira dois números inteiros para encontrar os coeficientes s e t da combinação linear de seu MDC: ");
    int scan_return = scanf("%d %d", &A, &D);

    if (scan_return != 2)
    {
        printf("❌ Os caracteres digitados formam uma entrada inválida!\n");
        return 0;
    }

    struct timespec start_time = start_clock();

    int s_value = 0, t_value = 0;
    int mdc = extended_euclides(A, D, &s_value, &t_value);

    printf("✅ Os coeficientes da combinação linear mdc(%d, %d) = %d são: %d e %d\n", A, D, mdc, s_value, t_value);

    end_clock(start_time);

    return 0;
}