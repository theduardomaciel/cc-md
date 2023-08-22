#include <stdio.h>
#include <stdbool.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 8. Escrever um programa para encontrar solução de uma congruência do tipo ax ≅ b mod m.

// Aplicamos o Teorema 5
int check_solutions_amount(int a, int b, int m)
{
    // d = mdc(a, m)
    int d = euclides(a, m);

    // Se o d divide b, então ax ≅ b mod m tem (valor do d) soluções, todas congruentes módulo m à sólução única de: a/d.x = b/d mod m/d
    if (d % b == 0)
    {
        return d;
    }
    else
    {
        // Se d não divide b, então a.x ≅ b mod m não tem solução
        return 0;
    }
}

// Teorema 4
// Se ã é a solução para a.x ≅ 1 mod m, então a solução para ax ≅ b mod m é x = b.ã

int main()
{
    struct timespec start_time = start_clock();

    int a, b, m;

    printf("🔢 >> Insira três inteiros (a, b e m) para encontrar a solução de uma congruência do tipo ax ≅ b mod m: ");
    int scan_return = scanf("%d %d %d", &a, &b, &m);

    if (scan_return != 3)
    {
        printf("❌ O input inserido não satisfaz o padrão: a b m\n");
        return 0;
    }

    // a.x ≅ b mod m
    // Para encontrar uma solução para essa congruência, primeiro precisamos antes encontrar a solução para a congruência: a.x ≅ 1 mod m, e, com isto, o inverso de a mod m

    int inverse = mod_inverse(a, m);

    if (inverse == 0)
    {
        printf("❌ Erro: Não foi possível encontrar uma solução para a congruência, visto que não existe um inverso modular para %d mod %d :(\n", a, m);
        return 0;
    }
    else
    {
        // O inverso modular que encontramos denomina-se s ou ã
        // Assim, realizamos b.ã para obtermos uma solução da congruência linear
        int solution = b * inverse;

        // Dividindo a solução por m, obteremos um resto
        int remaining = solution % m;

        // Este resto é a solução única para a congruência
        // A solução geral é: resto + m.k (sendo k um inteiro)
        printf("✅ A solução geral da congruência linear informada (%d.x %d mod %d) é: %d + %d.k (sendo k um inteiro)\n", a, b, m, remaining, m);
    }

    end_clock(start_time);

    return 0;
}