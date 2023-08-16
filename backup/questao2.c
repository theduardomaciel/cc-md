#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

// 2. Escrever um programa para listar números primos sequencialmente. Você deve otimizar o programa para que ele liste o maior número possivel de primos em 60 segundos.

void sieve(unsigned long long startValue, unsigned long long upperBound)
{
    int upperBoundSquareRoot = (int)sqrt((double)upperBound);

    // Precisamos de uma array de bits para armazenar n - 1 números: isComposite[2, 3, ... , n]
    // Alocamos a memória suficiente para todos os "bool" presente na array que possui como tamanho upperBound + 1
    bool *isComposite = (bool *)malloc((upperBound + 1) * sizeof(bool));

    if (isComposite == NULL)
    {
        perror("😢 Não foi possível alocar memória suficiente.");
        exit(EXIT_FAILURE);
    }

    // Substituímos todos os valores da array por falso
    for (int i = 0; i <= upperBound; i++)
    {
        isComposite[i] = false;
    }

    // Números que podem ser representados por k * p, em que "k" >= 2 e "p" é um primo, são marcados como números compostos ao atribuir 1 como valor em seu correspondente na array.
    // O algoritmo consiste em 2 loops, um dentro do outro.
    // O de fora procura por números primos, e o de dentro marca esses primos múltiplos como compostos
    /*
        For all numbers m: 2 .. √n, if m is unmarked:
            add m to primes list;
            mark all it's multiples, starting from square, lesser or equal than n (k * m ≤ n, k ≥ m);
        Otherwise, if m is marked, then it is a composite number;
        Check all numbers in range √n .. n. All found unmarked numbers are primes, add them to list.
    */

    for (int m = 2; m <= upperBoundSquareRoot; m++)
    {
        if (!isComposite[m])
        {
            int firstMultiple = (startValue / m) * m;
            if (firstMultiple < startValue)
                firstMultiple += m;
            for (int k = firstMultiple; k <= upperBound; k += m)
            {
                isComposite[k] = true;
            }
        }
    }

    int primeCount = 0; // Initialize prime count

    for (int m = (startValue <= upperBoundSquareRoot) ? upperBoundSquareRoot + 1 : startValue; m <= upperBound; m++)
    {
        if (!isComposite[m])
        {
            printf("%d. %d\n", ++primeCount, m);
        }
    }

    free(isComposite);
}

void runSieve()
{
    time_t start = time(NULL);
    time_t limit = 10;

    time_t endwait = start + limit;

    int necessaryLoops = INT_MAX / sizeof(bool);
    printf("coiso: %d\n", necessaryLoops);

    printf("Os números abaixo foram os calculados durante os 10 segundos de intervalo ⬇️\n");
    sieve(0, INT_MAX / 64);
}

int main()
{
    runSieve();
    return 0;
}

/*
    Sieve of Eratosthenes
    The Sieve of Eratosthenes is a standard benchmark used to determine the relative speed of different computers or, in this case, the efficiency of the code generated for the same computer by different compilers.
    The sieve algorithm was developed in ancient Greece and is one of a number of methods used to find prime numbers.
    The sieve works by a process of elimination using an array that starts with 2 and keeps all the numbers in position. The process is:

    2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

    Starting after 2, eliminate all multiples of 2.

    2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

    Starting after 3, eliminate all multiples of 3.

    2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

    Starting after 5, eliminate all multiples of 5.

    2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25

    Continue until the next remaining number is greater than the square root of the largest number in the original series. In this case, the next number, 7, is greater than the square root of 25, so the process stops. The remaining numbers are all prime.

    2 3 5 7 11 13 17 19 23
*/