#include <stdbool.h>

// Conceito: um inteiro "unsigned" em C abre mão dos números negativos para duplicar a capacidade de armazenamento de números positivos

bool isPrime(unsigned long number)
{
    for (unsigned long i = 2; i < (number / 2); i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }

    return true;
}

void decompose(unsigned int n)
{
    const unsigned int FIRST_PRIME = 2;

    printf("%u = ", n);

    if (n < FIRST_PRIME)
    {
        printf("%u\n", n);
    }

    unsigned int m = FIRST_PRIME;

    while (n > 1)
    {
        unsigned int i = 0;

        while (n % m == 0)
        {
            ++i;
            n /= m;
        }

        if (i != 0)
        {
            printf("%u", m);
            if (i != 1)
            {
                printf("^%u ", i);
            }

            if (n != 1)
            {
                putchar('*');
                putchar(' ');
            }
        }

        m = m == FIRST_PRIME ? 3 : m + 2;
    }
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