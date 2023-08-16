#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "../modules/timer.h"
#include "../modules/prime.h"

// 4. Escrever um programa para determinar o MDC e o MMC de dois inteiros com base em seus fatores primos.

int main()
{
    struct timespec start_time = start_clock();

    int input1, input2;

    printf("🔢 >> Insira dois números inteiros para determinar seus MDC e MMC: ");
    int scan_return = scanf("%d %d", &input1, &input2);

    if (scan_return != 2)
    {
        printf("❌ Os caracteres digitados formam uma entrada inválida!\n");
        return 0;
    }

    printf("Estes são os MDC e MMC dos dois números informados: ⬇️\n");

    int decomposed1[10000] = {0};
    int decomposed2[10000] = {0};

    int decomposed1_exponents[10000] = {0};
    int decomposed2_exponents[10000] = {0};

    int length1 = decompose(input1, decomposed1, decomposed1_exponents);
    int length2 = decompose(input2, decomposed2, decomposed2_exponents);

    /* printf("Decomposed 1:\n");
    for (int i = 0; i < length1; i++)
    {
        printf("%d - ", decomposed1[i]);
    }

    printf("\nEXPOENTS 1:\n");
    for (int i = 0; i < length1; i++)
    {
        printf("%d - ", decomposed1_exponents[i]);
    }

    printf("\nDecomposed 2:\n");
    for (int i = 0; i < length2; i++)
    {
        printf("%d - ", decomposed2[i]);
    }

    printf("\nEXPOENTS 2:\n");
    for (int i = 0; i < length2; i++)
    {
        printf("%d - ", decomposed2_exponents[i]);
    } */

    /* for (int i = 0; i < fmax(length1, length2); i++)
    {
        // Caso o fator primo exista na decomposição dos dois números
        if (decomposed1[i] == decomposed2[0])
        {

        }
        else
        {
            // Caso não exista na decomposição de um dos dois, teremos que considerar o expoente do número em falta como 0
        }
    } */

    // MDC
    int mdc = 1;
    printf("mdc(%d, %d) =", input1, input2);
    int max_length = fmax(length1, length2);

    for (int i = 0; i < max_length; i++)
    {
        if (i != 0 && i != max_length)
        {
            printf(" *");
        }

        // Caso o fator primo exista na decomposição dos dois números
        if (decomposed1[i] == decomposed2[i])
        {
            int smallest_exponent = fmin(decomposed1_exponents[i], decomposed2_exponents[i]);

            // Como os valores de decomposed1 e decomposed2 são iguais, podemos utilizar o decomposed1 como geral
            printf(" %d^%d", decomposed1[i], smallest_exponent);
            mdc *= (pow(decomposed1[i], smallest_exponent));
        }
        else
        {
            // Caso o número na posição a ser iterada seja diferente entre os dois, teremos que procurar por algum correspondente na array oposta, caso não encontremos, como estamos tratando do MDC, o expoente será 0
            int exponent = 0;
            if (length1 > length2)
            {
                // Percorreremos a array oposta (2) e procurar o elemento que está no index i da array 1, se acharmos, pegaremos o menor expoente
                for (int check = 0; check < length2; check++)
                {
                    // printf("\n\t->Decomposto 2 (%d): %d e decomposed 1 (%d): %d\n", check, decomposed2[check], i, decomposed1[i]);
                    if (decomposed2[check] == decomposed1[i])
                    {
                        exponent = fmin(decomposed1_exponents[i], decomposed2_exponents[check]);
                        // printf("\t\tExpoente escolhido entre (%d) e (%d): %d\n", decomposed1_exponents[i], decomposed2_exponents[check], exponent);
                        break;
                    }
                }
                printf(" %d^%d", decomposed1[i], exponent);
                mdc *= (pow(decomposed1[i], exponent));
            }
            else
            {
                // O fator primo existe para o número 2 na iteração atual, vamos descobrir se ele também está presente na array do número 1
                for (int check = 0; check < length1; check++)
                {
                    // printf("\n\t->Decomposto 1 (%d): %d e decomposed 2 (%d): %d\n", check, decomposed1[check], i, decomposed2[i]);
                    if (decomposed1[check] == decomposed2[i])
                    {
                        exponent = fmin(decomposed1_exponents[check], decomposed2_exponents[i]);
                        // printf("\t\tExpoente escolhido entre (%d) e (%d): %d\n", decomposed1_exponents[check], decomposed2_exponents[i], exponent);
                        break;
                    }
                }
                printf(" %d^%d", decomposed2[i], exponent);
                mdc *= (pow(decomposed2[i], exponent));
            }
        }
    }

    printf(" = %d", mdc);

    end_clock(start_time);

    return 0;
}

/*
 no caso do MDC, sempre considerar os discrepantes como x^0 = 1, visto que queremos obter os menores expoentes entre os dois números
            printf(" * %d^0", decomposed1[i]);
            printf(" * %d^0", decomposed1[i]);
             */