#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include <gmp.h>
// #include <emscripten.h>

// ================= FUNÇÕES AUXILIARES ==============================

/* EMSCRIPTEN_KEEPALIVE */
long int mdc(long int a, long int b)
{
    if (b == 0)
    {
        return a;
    }
    else
    {
        return mdc(b, a % b);
    }
}

long int isPrime(long int n)
{
    if (n == 2 || n == 3)
    {
        return 1;
    }

    for (long int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }

    if (n != 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// ===============================================

// A chave pública é constituída por dois elementos: n e e
// O elemento n é o produto de dois números primos p e q
// O elemento e é um expoente co-primo com o totiente de n
// O elemento d é o inverso multiplicativo de e módulo o totiente de n

// O par (n, e) forma a chave pública.
// O par (n, d) forma a chave privada.

/*
    TABELA DE ERROS: "n_factor"
        -1 = o valor de p ou q ultrapassa os limites da linguagem C
        -2 = o valor do produto de p e q é menor que 256, ou seja, a criptografia não é suficientemente segura
        -3 = o valor de p não é primo
        -4 = o valor de q não é primo
        -5 = o valor de p ou q não é primo
*/

/*
    CONCEITO MATEMÁTICO:
    "p" e "q" são dois números primos grandes distintos. Esses números são mantidos em segredo e não são compartilhados.
*/

long int verifications(long int p, long int q)
{
    long int is_p_prime = isPrime(p);
    long int is_q_prime = isPrime(q);

    if (p > INT_MAX || q > INT_MAX)
    {
        return -1;
    }
    else if (p * q < 256)
    {
        return -2;
    }
    else if (!is_p_prime && is_q_prime)
    {
        return -3;
    }
    else if (!is_q_prime && is_p_prime)
    {
        return -4;
    }
    else if (!is_p_prime && !is_q_prime)
    {
        return -5;
    }
    else
    {
        return 1;
    }
}

// ELEMENTO DA CHAVE PÚBLICA E PRIVADA
/* EMSCRIPTEN_KEEPALIVE */
unsigned long int n_factor(long int p, long int q)
{
    long int verificationsResult = verifications(p, q);

    if (verificationsResult < 0)
    {
        return verificationsResult;
    }
    else
    {
        printf("p: %ld e q: %ld", p, q);
        return p * q;
    }
}

/*
    TABELA DE ERROS: "publicKey_totient"
        -1 = o valor de p ou q ultrapassa os limites da linguagem C
        -3 = o valor de p não é primo
        -4 = o valor de q não é primo
*/

/*
    CONCEITO MATEMÁTICO:
    O totiente de um número inteiro positivo n é denotado como φ(n)
    "Por exemplo, se n for um número primo, então φ(n) será igual a n - 1, pois todos os inteiros positivos menores que n são coprimos com n.
    Por outro lado, se n for o produto de dois números primos distintos, como n = p * q, onde p e q são primos diferentes,
    então φ(n) será igual a (p - 1) * (q - 1), porque apenas os múltiplos de p, os múltiplos de q e seus múltiplos comuns não são coprimos com n."

    Portanto, para calcular a chave privada utilizamos a função totiente de Euler calculando: φ(n) = (p - 1) * (q - 1)
*/

/* EMSCRIPTEN_KEEPALIVE */
unsigned long int publicKey_totient(long int p, long int q)
{
    long int verificationsResult = verifications(p, q);

    if (verificationsResult < 0)
    {
        return verificationsResult;
    }
    else
    {
        printf("p: %ld e q: %ld", p, q);
        return (p - 1) * (q - 1);
    }
}

/*
    CONCEITO MATEMÁTICO:
    Escolhemos um número inteiro positivo e relativamente primo (co-primo) a φ(n), geralmente chamado de "e" (exponente de encriptação)
    O valor de e deve ser menor do que φ(n) e maior do que 1.
*/

// 2ª ELEMENTO DA CHAVE PÚBLICA
/* EMSCRIPTEN_KEEPALIVE */
long int publicKey_e(long int totient, long int initialExponent)
{
    // Caso estejamos em uma requisição da função em loop, continuamos com base no último expoente retornado + 1
    // Caso contrário, começamos com o expoente 2, visto que o expoente deve ser maior que 1
    long int exponent = initialExponent > 0 ? initialExponent + 1 : 2;

    while (mdc(exponent, totient) != 1)
    {
        exponent++;
    }

    return exponent;
}

/*
    CONCEITO MATEMÁTICO:
    Calculamos a chave privada d, que é o inverso multiplicativo de e modulo φ(n).
    Isso significa encontrar um número d tal que (d * e) % φ(n) = 1
*/

/* EMSCRIPTEN_KEEPALIVE */
long int privateKey_d(long int totient, long int exponent)
{
    long int d = 1;
    while ((d * exponent) % totient != 1)
    {
        d++;
    }

    return d;
}

// ======================= ENCRIPTAÇÃO DE MENSAGENS  ====================================

// FUNÇÕES AUXILIARES

/*
    ASSERT:
        Ao executar o programa, se a expressão for verdadeira (true), o assert se mantém silencioso, ou seja, nada acontecerá.
        Mas se ao executar o programa a expressão for falsa (false), o assert interromperá a execução do programa.

    OPERADOR DE DESLOCAMENTO À DIREITA (>>)
        A operação exponent >> 1, por exemplo, desloca todos os bits de exponent uma posição para a direita, o que é equivalente a dividir o valor por 2 (ou seja, realizar uma divisão inteira por 2).
        O uso de deslocamento à direita (>>) é preferido em relação à divisão normal por 2, especialmente quando estamos trabalhando com números inteiros, pois é uma operação mais eficiente do ponto de vista computacional.
*/

/*
    FUNÇÃO DE EXPOENCIAÇÃO MODULAR RÁPIDA
        - (links importantes: https://en.wikipedia.org/wiki/Modular_exponentiation & https://en.wikipedia.org/wiki/Exponentiation_by_squaring)

    Observação: esse algoritmo pode não ser adequado para a manipuação de números muito grandes, como os usados em chaves RSA de 2048 bits
*/

unsigned long long int modular_pow(unsigned long long int base, unsigned long long int exponent, unsigned long long int modulus)
{
    if (modulus * modulus > ULLONG_MAX)
    {
        printf("Overflow");
        return -1;
    }

    unsigned long long int result = 1;
    base = base % modulus;

    while (exponent > 0)
    {
        if (exponent % 2 == 1)
        {
            result = (result * base) % modulus;
        }
        exponent = exponent >> 1; // dividimos o valor do expoente pela metade
        base = (base * base) % modulus;
    }

    return result;
}

int needs_gmp(unsigned long long int num)
{
    mpz_t mpz_num;
    mpz_init_set_ui(mpz_num, num); // Convertemos o número para um mpz_t

    int result = mpz_sizeinbase(mpz_num, 2) > 64; // Verificamos, na base 2, se o número é maior que 64 bits

    mpz_clear(mpz_num);

    return result; // Se o número for maior que 64 bits, retornamos 1 (a biblioteca GMP deve ser usada), caso contrário, retornamos 0
}

// Função para converter um mpz_t para um unsigned long long int
unsigned long long mpz2ull(mpz_t z)
{
    unsigned long long result = 0;
    mpz_export(&result, 0, -1, sizeof result, 0, 0, z);
    return result;
}

// Função para detectar se um mpz_t ultrapassa os limites de unsigned long long int
int mpz2ull_overflow(mpz_t z)
{
    unsigned long long result = 0;
    mpz_export(&result, 0, -1, sizeof result, 0, 0, z);
    return result == ULLONG_MAX && mpz_cmp_ui(z, ULLONG_MAX) > 0;
}

/*
    CONCEITO MATEMÁTICO:
    A mensagem é encriptada em blocos de tamanho fixo, onde cada bloco é um número inteiro entre 0 e n - 1.
    Para encriptar um bloco m, calculamos o resto da divisão de m elevado a e por n.
    Ou seja, c = (m ^ e) % n
*/

/*
    OBSERVAÇÕES DA BIBLIOTECA GMP:
        1. "GMP integers are stored in objects of type mpz_t."
        2. "Passing a zero divisor to the division or modulo functions (including the modular powering functions mpz_powm and mpz_powm_ui) will cause an intentional division by zero."
        3. mpz_inits: "initialize a NULL-terminated list of mpz_t variables, and set their values to 0".
        4. mpz_powm: "Set rop to base raised to exp modulo mod".
*/

/* EMSCRIPTEN_KEEPALIVE */
/* char *RSA_encrypt(const char *message, const char *e_string, const char *n_string, char *result)
{
    mpz_t msg, e, n, encrypted;
    mpz_inits(msg, e, n, encrypted, NULL); // mpz_inits: initialize a NULL-terminated list of mpz_t variables, and set their values to 0.

    // Convertemos os valores para a base decimal (10)
    mpz_set_str(msg, message, 10);
    mpz_set_str(e, e_string, 10);
    mpz_set_str(n, n_string, 10);

    // Comos os processos matemáticos de criptografia devem ser feitos manualmente, não utilizaremos a função mpz_powm enquanto for possível
    if (mpz2ull_overflow(e) || mpz2ull_overflow(n))
    {
        // Nos casos de números grandes demais, utilizamos a função mpz_powm
        mpz_powm(encrypted, msg, e, n);
        mpz_get_str(result, 10, encrypted);
    }
    else
    {
        // Para encriptar, utilizaremos o método de exponenciação modular rápida
        // Primeiramente convertemos os valores de mpz_t para long int
        unsigned long long int base = mpz2ull(msg);
        unsigned long long int exponent = mpz2ull(e);
        unsigned long long int modulus = mpz2ull(n);

        // Em seguida, utilizamos a função de exponenciação modular rápida
        unsigned long long int int_result = modular_pow(base, exponent, modulus);

        // E convertemos o resultado de volta para um mpz_t, que será atribuido à variável "encrypted".
        mpz_set_ui(encrypted, int_result);

        // Por fim, convertemos o valor de "encrypted" para uma string e retornamos o resultado.
        // char *result_string = mpz_get_str(NULL, 10, encrypted);
        mpz_get_str(result, 10, encrypted);

        // return result_string;

        // Não podemos limpar o valor de "encrypted" pois ele será retornado
        mpz_clears(msg, e, n, encrypted, NULL); // mpz_clears: Free the space occupied by a NULL-terminated list of mpz_t variables.
    }
}
 */

char *RSA_encrypt(const char *message, const char *e_string, const char *n_string, char *result)
{
    mpz_t current_char, e, n, current_encrypted_int;
    mpz_inits(current_char, e, n, current_encrypted_int, NULL);

    // Convertemos os valores mpz_t (strings de números bem grandes) para a base decimal (10)
    mpz_set_str(e, e_string, 10);
    mpz_set_str(n, n_string, 10);

    size_t message_length = strlen(message);

    // Loopamos por cada caractere da mensagem
    for (size_t i = 0; i < message_length; i++)
    {
        // Traduzimos o valor do caractere em um inteiro obtendo o correspondente do caractere em numeral ASCII
        long int ascii_value = (long int)(message[i]);

        // Alteramos o valor de "current_char" (em mpz_t) para o valor do caractere em numeral ASCII
        mpz_set_ui(current_char, ascii_value);

        // Realizamos a criptografia encontrando o equivalente cifrado "C" de "m" (letra atual convertida em inteiro) a partir da seguinte função:
        // C = (m ^ e) % n (m = caractere atual, e = expoente, n = produto de p e q)
        mpz_powm(current_encrypted_int, current_char, e, n);

        // Convertemos o valor de "current_encrypted_int" para uma string (visto que o número pode ser bem grande)...
        char encrypted_char[1024];
        mpz_get_str(encrypted_char, 10, current_encrypted_int);

        strcat(result, encrypted_char); // ...e concatenamos o resultado com a variável "result"

        if (i < message_length - 1)
        {
            strcat(result, " "); // Adicionamos espaço ENTRE os números
        }
    }

    mpz_clears(current_char, e, n, current_encrypted_int, NULL);
}

// ======================= DESENCRIPTAÇÃO DE MENSAGENS  ====================================

/*
    CONCEITO MATEMÁTICO
    Para desencriptar a mensagem, calculamos o resto da divisão de c elevado a d por n.
    Ou seja, m = (c ^ d) % n
*/

/* EMSCRIPTEN_KEEPALIVE */
/* char *RSA_decrypt(const char *encrypted_message, const char *d_string, const char *n_string, char *result)
{
    mpz_t encrypted, d, n, decrypted;
    mpz_inits(encrypted, d, n, decrypted, NULL);

    // Convertemos os valores para a base decimal (10)
    mpz_set_str(encrypted, encrypted_message, 10);
    mpz_set_str(d, d_string, 10);
    mpz_set_str(n, n_string, 10);

    // Comos os processos matemáticos de criptografia devem ser feitos manualmente, não utilizaremos a função mpz_powm enquanto for possível
    if (mpz2ull_overflow(d) || mpz2ull_overflow(n))
    {
        // Nos casos de números demasiadamente grandes, utilizamos a função mpz_powm
        mpz_powm(decrypted, encrypted, d, n);
        mpz_get_str(result, 10, decrypted);
    }
    else
    {
        // Para desencriptar, utilizaremos o método de exponenciação modular rápida
        // Primeiramente convertemos os valores de mpz_t para long int
        unsigned long long int base = mpz2ull(encrypted);
        unsigned long long int exponent = mpz2ull(d);
        unsigned long long int modulus = mpz2ull(n);

        // Em seguida, utilizamos a função de exponenciação modular rápida
        unsigned long long int int_result = modular_pow(base, exponent, modulus);

        // E convertemos o resultado de volta para um mpz_t, que será atribuido à variável "decrypted".
        mpz_set_ui(decrypted, int_result);

        // Por fim, convertemos o valor de "decrypted" para uma string e retornamos o resultado.
        // char *result_string = mpz_get_str(NULL, 10, decrypted);
        mpz_get_str(result, 10, decrypted);

        // return result_string;

        // Não podemos limpar o valor de "decrypted" pois ele será retornado
        mpz_clears(encrypted, d, n, decrypted, NULL); // mpz_clears: Free the space occupied by a NULL-terminated list of mpz_t variables.
    }
}
 */

/*
    PARA TESTES:
        Chave Privada: {
            d = 44273
            n = 49163
        }

        Chave Pública {
            e = 65537
            n = 49163
        }
*/

int main()
{
    const char *message = "testando"; // mensagem original
    const char *e_str = "65537";      // valor de e
    const char *d_str = "44273";      // valor de d
    const char *n_str = "49163";      // valor de n

    char encrypted_result[1024] = {0};
    // char decrypted_result[1024] = {0};

    RSA_encrypt(message, e_str, n_str, encrypted_result);
    // RSA_decrypt(encrypted_result, d_str, n_str, decrypted_result);

    printf("Mensagem Original: %s\n", message);
    printf("Mensagem Criptografada: %s\n", encrypted_result);
    // printf("Mensagem Descriptografada: %s\n", decrypted_result);

    return 0;
}