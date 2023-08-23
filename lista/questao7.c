#include <stdio.h>
#include <stdbool.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 7. Escrever um programa para encontrar o inverso de a mod b.

/*
    A X ≅ 1 (mod M)
    Nota: O valor de X deve estar no interfalo {1, 2, … M-1}

    O inverso multiplicativo de “A mod M” existe se, e somente se, A e M são relativamente primos, ou seja, coprimos (mdc(A, M) = 1).
*/

int main()
{
    int A, B;

    printf("🔢 >> Insira dois números inteiros para encontrar o inverso de a mod b: ");
    int scan_return = scanf("%d %d", &A, &B);

    if (scan_return != 2)
    {
        printf("❌ Os caracteres digitados formam uma entrada inválida!\n");
        return 0;
    }

    struct timespec start_time = start_clock();

    int inverse = mod_inverse(A, B);

    if (inverse == 0)
    {
        printf("❌ Não existe um inverso modular para %d mod %d :(\n", A, B);
    }
    else
    {
        printf("✅ O inverso modular de %d mod %d é: %d\n", A, B, inverse);
    }

    end_clock(start_time);

    return 0;
}