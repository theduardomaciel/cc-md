#include <math.h>
#include <stdbool.h>

// Conceito: um inteiro "unsigned" em C abre mão dos números negativos para duplicar a capacidade de armazenamento de números positivos

bool isPrime(unsigned long number)
{
    // Percorremos todos os inteiros i, variando de 2 a n - 1, e então testamos se i é divisor de n
    // No entanto, de acordo com a Teoria do Número Primo, temos que "o numero de primos n ̃ao excedentes a x é aproximadamente x / ln(x)"
    for (unsigned long i = 2; i < (number / log(number)); i++)
    {
        if (number % i == 0)
        {
            // se i é divisor, n é composto
            return false;
        }
    }

    // se nenhum i for divisor, n é primo
    return true;
}

int decompose(int number, int array[], int expoents_array[])
{
    const int FIRST_PRIME = 2;

    if (number < FIRST_PRIME)
    {
        return 0; // Retorna 0 se o número for menor que 2, pois não é possível decompor.
    }

    int current = current;
    int total = 0; // será uasada para manter o índice atual nos arrays de fatores primos e expoentes.

    // O loop continua até que number seja reduzido a sua raiz quadrada, pois após esse ponto, os fatores restantes já foram identificados.
    while (number > 1)
    {
        int i = 0;

        // current é um fator primo do número.
        while (number % current == 0)
        {
            ++i;
            number /= current;
        }

        if (i != 0)
        {
            // printf("%d", current);
            array[total] = current;

            if (i != 1)
            {
                // printf("^%d", i);
                expoents_array[total] = i;
            }

            /* if (number != 1)
            {
                printf(" * ");
            } */
            total += 1;
        }

        current = current == FIRST_PRIME ? 3 : current + 2;
    }

    return total;
}

int frequency_count(int arr[], int num, int size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (num == arr[i])
        {
            count++;
        }
    }
    return count;
}