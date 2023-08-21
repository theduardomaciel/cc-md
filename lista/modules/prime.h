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

int euclides(int A, int D)
{
    do
    {
        int r = A % D;
        A = D;
        D = r;
    } while (A % D != 0);

    return D;
}

int extended_euclides(int A, int D, int* s, int* t)
{
    //printf("s atual: %d e t atual: %d\n", *s, *t);

    // Caso base
    if (A == 0)
    {
        *s = 0;
        *t = 1;
        return D;
    }

    int s1 = 0, t1 = 0; // Utilizamos as variáveis para armazenar os resultados das chamadas recursivas
    int mdc = extended_euclides(D % A, A, &s1, &t1);

    // Atualizamos s e t utilizando os resultados das chamadas recursivas
    *s = t1 - (D /A ) * s1;
    *t = s1;

    //printf("s novo: %d e t novo: %d\n", t1 - (D /A ) * s1, s1);

    return mdc;
}

/* 
    // C program to demonstrate working of extended Euclidean Algorithm
    #include <stdio.h>

    // C function for extended Euclidean Algorithm
    int gcdExtended(int a, int b, int *x, int *y)
    {
        // Base Case
        if (a == 0)
        {
            *x = 0;
            *y = 1;
            return b;
        }

        int s1, t1; // To store results of recursive call
        int gcd = gcdExtended(b%a, a, &s1, &t1);

        // Update x and y using results of recursive
        // call
        *x = t1 - (b/a) * s1;
        *y = s1;

        return gcd;
    }

    // Driver Program
    int main()
    {
        int x, y;
        int a = 35, b = 15;
        int g = gcdExtended(a, b, &x, &y);
        printf("gcd(%d, %d) = %d", a, b, g);
        return 0;
    }
*/

/* 
    Time Complexity: O(log N)
    Auxiliary Space: O(log N)

    Como o Algoritmo de Euclides extendido funciona?
    ------------------------------------------------

    As seen above, x and y are results for inputs a and b,

    a.x + b.y = mdc                      — (1)  

    And s1 and t1 are results for inputs b % a and a

        (b % a).s1 + a.t1 = mdc   

    When we put b % a = (b – (⌊b/a⌋).a) in above, we get following: (Note that ⌊b/a⌋ is floor(b/a)

        (b – (⌊b/a⌋).a).s1 + a.t1  = mdc

    Above equation can also be written as below
    b.s1 + a.(t1 – (⌊b/a⌋).s1) = mdc      — (2)

    After comparing coefficients of ‘a’ and ‘b’ in (1) and (2), we get following, 
    x = t1 – ⌊b/a⌋ * s1
    y = s1
*/