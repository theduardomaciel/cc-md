#include <stdio.h>
#include <stdbool.h>
#include "../modules/timer.h"

// 1. Escrever um programa que determina se um dado número inteiro é primo ou não.

// Conceito: um inteiro "unsigned" em C abre mão dos números negativos para duplicar a capacidade de armazenamento de números positivos

bool isPrime(unsigned long long number)
{
    printf("⏳ Verificando se %llu é um número primo...\n", number);
    for (unsigned long long i = 2; i < number; i++)
    {
        if (number % i == 0)
        {
            return false;
        }
    }

    return true;
}

int main()
{
    struct timespec start_time = start_clock();

    unsigned long long input;

    printf("🔢 >> Insira um número inteiro para verificar se é primo: ");
    int scan_return = scanf("%llu", &input);

    if (scan_return != 1)
    {
        printf("❌ O caractere digitado não é um número válido!\n");
        return 0;
    }

    bool input_isPrime = isPrime(input);

    if (input_isPrime)
    {
        printf("✅ O número inserido É PRIMO! 🥳\n");
    }
    else
    {
        printf("⛔ Opa. O número inserido NÃO É PRIMO!\n");
    }

    end_clock(start_time);

    return 0;
}