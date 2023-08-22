#include <math.h>
#include <stdbool.h>

// Conceito: um inteiro "unsigned" em C abre mão dos números negativos para duplicar a capacidade de armazenamento de números positivos

bool isPrime(int number)
{
    if (number < 1)
        return false;
    if (number == 2)
        return true;

    // Percorremos todos os inteiros i, variando de 3 à raiz quadrada de number, e então testamos se i é divisor de n
    for (int i = 3; i < sqrt(number); i += 2)
    {
        if (!(number % i))
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

// MDC(A, 0) = A
// MDC(0, D) = D
// Se A = D⋅ Q + R e B ≠ 0, então MDC(A,B) = MDC(B,R) sendo Q um inteiro, e R um inteiro entre 0 e B - 1

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

// gcd = mmc em inglês

/*
    a.s + b.t = mdc(a, b)
    mdc(a, b) = mdc(b % a, a)
    mdc(b % a, a) = (b % a).s1 + a.t1
    a.s + b.t = (b%a).s1 + a.t1
    a.s + b.t = (b – [b/a] * a).s1 + a.t1
    a.s + b.t = a(t1 – [b/a] * s1) + b.s1

    Comparing LHS and RHS,
    s = t1 – ⌊b/a⌋ * s1
    t = s1
*/

int extended_euclides(int A, int D, int *s, int *t)
{
    // Inicializamos os coeficientes de Bézout
    int s0 = 1, s1 = 0, t0 = 0, t1 = 1;
    // Inicializamos as variáveis
    int q, r, m, n;

    while (A > 0)
    {
        // A cada loop, calculamos o quociente (q) e o resto (r) da divisão de D por A
        q = D / A;
        r = D % A;

        // Armazenamos temporariamente os coeficientes calculados nessa iteração
        m = s0 - q * s1;
        n = t0 - q * t1;

        // Atualizamos as variáveis para a próxima iteração
        D = A;
        A = r;
        s0 = s1;
        t0 = t1;
        s1 = m;
        t1 = n;
    }

    // Após o loop, os coeficientes finais são armazenados em *s e *t
    *s = t0;
    *t = s0;

    return D; // Retornamos o MDC
}

/*
    Time Complexity: O(log N)
    Auxiliary Space: O(log N)

    Como o Algoritmo de Euclides extendido funciona?
    ------------------------------------------------

    Como visto, s e t são resultados para os inputs a e b,

    a.s + b.t = mdc                      — (1)

    E s1 e t1 são resultados para os inputs b % a e a

        (b % a).s1 + a.t1 = mdc

    Quando colocamos b % a = (b – (⌊b/a⌋).a) acima, obtemos o seguinte: (⌊b/a⌋ é o floor(b/a))

        (b – (⌊b/a⌋).a).s1 + a.t1  = mdc

    A equação acima também pode ser escrita da seguinte maneira:
    b.s1 + a.(t1 – (⌊b/a⌋).s1) = mdc      — (2)

    // Depois de comparar os coeficientes de 'a' e 'b' em (1) e (2), obtemos o seguinte:
    s = t1 – ⌊b/a⌋ * s1
    t = s1
*/

// Função para encontrar o inverso modular de a
int mod_inverse(int A, int M)
{
    int s, t;
    int mdc = extended_euclides(A, M, &s, &t);
    if (mdc != 1)
        return 0;
    else
    {
        // O coeficiente de A em uma combinação linear entre A e M que seja igual a 1 é um inverso de a módulo m.
        // Logo, s, o coeficiente de A, é o inverso de A módulo M.
        // Além disso, todo número congruente à: "s mod M" também é um inverso de "A mod M"

        // Somamos M para lidar com caso do coeficiente s ser negativo
        return (s % M + M) % M;
    }
}