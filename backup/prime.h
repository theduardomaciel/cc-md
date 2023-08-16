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

// Pede dois números como argumento, e retorna a potência do segundo número na decomposição em primos do primeiro (tipo, quantas vezes eu posso dividir o primeiro pelo segundo?)
int expotent(int n, int p)
{
    int j = 0;
    while ((n % p) == 0)
    {
        j = j + 1;
        p = p * p;
    }
    return j;
}

int decompose(unsigned long number, unsigned long array[])
{
    unsigned long total_decomposed_amount = 0;
    for (unsigned long i = 2; i <= number; i++)
    {
        while (number % i == 0)
        {
            // printf("%lu\n", i);
            array[total_decomposed_amount] = i;

            number = number / i;
            total_decomposed_amount += 1;
        }
    }
    return total_decomposed_amount;
}

int getDecompositionPower(unsigned long array[], unsigned long length, unsigned long desiredNumber)
{
    int power = 0;
    for (unsigned long i = 0; i < length; i++)
    {
        if (array[i] == desiredNumber)
        {
            power += 1;
        }
    }
    return power;
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

/*
    int i = 2, p;
    while (number > 1)
    {
        // checamos se i é primo
        bool numberIsPrime = isPrime(i);

        if (numberIsPrime) // if i is prime
        {
            p = 0;
            while (number % i == 0) /// contamos quantas vezes i é um fator
            {
                p = p + 1;
                number = number / i;
            }

            if (p != 0)
            {
                printf("%d %d\n", i, p);
            }
        }
        i = i + 1;
    }
    return 0;
*/