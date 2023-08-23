#include <stdio.h>
#include <math.h>
#include <time.h>

#include "./modules/timer.h"
#include "./modules/prime.h"

// 2. Escrever um programa para listar números primos sequencialmente. Você deve otimizar o programa para que ele liste o maior número possivel de primos em 60 segundos.

int getBiggestPrime()
{
    struct timespec begin = start_clock();

    int currentNumber = 2;
    int primesCount = 1;

    // Como 2 é o único número par primo, imprimimos ele individualmente
    printf("2\n");

    while (get_time_in_seconds(begin) <= 10)
    {
        if (isPrime(currentNumber))
        {
            printf("%d. %d\n", primesCount, currentNumber);
            //printf("%d\n", currentNumber);
            ++primesCount;
        }
        currentNumber += 2;
    }
}

int main()
{
    printf("Os números abaixo foram os calculados durante os 10 segundos de intervalo ⬇️\n");
    getBiggestPrime();
    return 0;
}