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

    int total = 0; // será uasada para manter o índice atual nos arrays de fatores primos e expoentes.

    /*
        -> Lidando com o fator primo 2:
        Esse loop está focado em lidar especificamente com o fator primo 2.
        Ele é uma otimização para lidar com divisões repetidas por 2, que é o único número par que é primo.
        Dividir o número por 2 repetidamente até que o número não seja mais divisível por 2 (ou seja, quando o resto da divisão não é mais zero) permite identificar o número de vezes que o fator 2 está presente na decomposição em fatores primos do número.
        Isso permite que o algoritmo otimize a decomposição do fator 2 antes de continuar com outros fatores primos ímpares.
    */

    // Verificamos se o número for par, caso seja, significa que o fator primo 2 está presente na decomposição
    while (number % 2 == 0)
    {
        // O fator primo 2 é adicionado à array, indicando que encontramos um fator primo 2.
        array[total] = 2;
        expoents_array[total] = 0; // inicializamos o expoents_array correspondente ao fator 2 como 0, para incrementar logo em seguida

        // O expoents_array correspondente ao fator 2 é incrementado para contar quantas vezes o fator 2 está presente na decomposição.
        while (number % 2 == 0)
        {
            number /= 2;             // dividimos repetidamente o número por 2 enquanto ele for divisível por 2
            expoents_array[total]++; // cada vez que essa divisão é realizada, incrementamos o contador de expoentes
        }

        total++; // incrementamos para preparar a próxima posição para armazenar outros fatores primos e seus expoentes
    }

    int current = 3;

    // A iteração por fatores primos é limitada até a raiz quadrada do número, o que reduz a complexidade do algoritmo, pois após esse ponto, os fatores restantes já foram identificados.
    while (current <= sqrt(number))
    {
        if (number % current == 0)
        {
            array[total] = current;
            expoents_array[total] = 0;

            while (number % current == 0)
            {
                number /= current;
                expoents_array[total]++;
            }

            total++;
        }

        current += 2; // Próximo número ímpar
    }

    if (number > 1)
    {
        array[total] = number;
        expoents_array[total] = 1;
        total++;
    }

    return total; // Retorna o total de fatores primos encontrados
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