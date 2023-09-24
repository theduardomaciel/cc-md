#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

// #include <emscripten.h>
#include "gmp.h"

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

    OBSERVAÇÕES DA BIBLIOTECA GMP:
        1. "GMP integers are stored in objects of type mpz_t."
        2. "Passing a zero divisor to the division or modulo functions (including the modular powering functions mpz_powm and mpz_powm_ui) will cause an intentional division by zero."
        3. mpz_inits: "initialize a NULL-terminated list of mpz_t variables, and set their values to 0".
        4. mpz_powm: "Set rop to base raised to exp modulo mod".
*/

/*
    FUNÇÃO DE EXPOENCIAÇÃO MODULAR RÁPIDA (utilizando "produto de quadrados")
        - (links importantes: https://en.wikipedia.org/wiki/Modular_exponentiation & https://en.wikipedia.org/wiki/Exponentiation_by_squaring)
        - Observação: esse algoritmo pode não ser adequado para a manipuação de números muito grandes, como os usados em chaves RSA de 2048 bits
*/

unsigned long long int modular_pow(unsigned long long int base, unsigned long long int exponent, unsigned long long int mod)
{
    if (mod <= 1)
    {
        return 0;
    }

    unsigned long long int result = 1;
    base = base % mod;

    while (exponent > 0)
    {
        // Se o expoente for ímpar, multiplicamos a base ao resultado.
        if (exponent % 2 == 1)
        {
            result = (result * base) % mod;
        }

        // Agora, o expoente é sempre par, então podemos dividir a base ao quadrado.
        base = (base * base) % mod;
        exponent = exponent >> 1; // dividimos o valor do expoente pela metade
    }

    return result;
}

// FUNÇÃO ADAPTADA PARA A BIBLIOTECA GMP
/* void modular_pow(mpz_t result, const mpz_t base, const mpz_t exponent, const mpz_t modulus)
{
    // Criamos uma cópia do valor base para manipulação durante o cálculo
    mpz_t base_copy;
    mpz_init(base_copy);
    mpz_set(base_copy, base);

    // Inicializamos o resultado como 1
    mpz_set_ui(result, 1);

    // Realizamos a exponenciação modular
    while (mpz_cmp_ui(exponent, 0) > 0)
    {
        // Se o expoente for ímpar, multiplicamos o resultado pelo valor base e fazemos a redução modular
        if (mpz_odd_p(exponent))
        {
            mpz_mul(result, result, base_copy);
            mpz_mod(result, result, modulus);
        }

        // Elevamos o valor base ao quadrado e fazemos a redução modular
        mpz_mul(base_copy, base_copy, base_copy);
        mpz_mod(base_copy, base_copy, modulus);

        // Dividimos o expoente por 2 (deslocamento à direita) para continuar a iteração
        mpz_divexact_ui(exponent, exponent, 2);
    }

    // Liberamos a memória alocada para a cópia do valor base
    mpz_clear(base_copy);
}
 */

void RSA_encrypt(const char *message, const char *e_string, const char *n_string, char **result)
{
    mpz_t msg, e, n, encrypted;
    mpz_inits(msg, e, n, encrypted, NULL);

    // Configuramos a chave pública, expressa em valores mpz_t (strings de números bem grandes), para a base decimal (10)
    mpz_set_str(e, e_string, 10);
    mpz_set_str(n, n_string, 10);

    size_t msg_len = strlen(message);       // Obtemos o tamanho da mensagem de entrada
    *result = (char *)malloc(msg_len * 20); // Alocamos memória para armazenar a mensagem criptografada (20 é um tamanho aproximado)

    if (*result == NULL)
    {
        fprintf(stderr, "Erro na alocação de memória.\n");
        exit(1);
    }

    char *current_position = *result; // Ponto de partida para escrever na memória alocada

    for (size_t i = 0; i < msg_len; i++)
    {
        unsigned char current_char = (unsigned char)(message[i]); // Obtemos o caractere atual
        mpz_set_ui(msg, current_char);                            // Configuramos o número mpz_t com o valor ASCII do caractere

        /*
            Para limitar os caracteres à tabela ASCII, utilizaríamos o seguinte:

            // Traduzimos o valor do caractere em um inteiro obtendo o correspondente do caractere em numeral ASCII
            long int ascii_value = (long int)(message[i]);

            // Alteramos o valor de "current_char" (em mpz_t) para o valor do caractere em numeral ASCII
            mpz_set_ui(msg, ascii_value);
        */

        // ========

        // Realizamos a criptografia RSA encontrando o equivalente cifrado "C" a partir da seguinte função:
        // C = (m ^ e) % n (m = caractere atual convertido em numeral ASCII, e = expoente, n = produto de p e q)

        // Verificamos se os valores são grandes demais para serem manipulados pela nossa função de exponenciação modular rápida
        mpz_t multiplication;
        mpz_init(multiplication);

        mpz_mul(multiplication, n, n);

        // Caso seja possível, utilizamos nossa função própria de exponenciação modular rápida
        if (mpz_fits_ulong_p(multiplication))
        {
            // printf("Fits ulong\n");
            unsigned long long int msg_int = mpz_get_ui(msg);
            unsigned long long int e_int = mpz_get_ui(e);
            unsigned long long int n_int = mpz_get_ui(n);

            unsigned long long int int_result = modular_pow(msg_int, e_int, n_int);

            // E convertemos o resultado de volta para um mpz_t, que será atribuido à variável "encrypted".
            mpz_set_ui(encrypted, int_result);
        }
        else
        {
            // Caso não seja possível, utilizamos a função mpz_powm
            mpz_powm(encrypted, msg, e, n);
        }

        mpz_clear(multiplication); // Liberamos a memória usada pelo número mpz_t

        // ========

        char encrypted_char[1024];                  // Criamos um Buffer para armazenar a representação em string do número criptografado
        mpz_get_str(encrypted_char, 10, encrypted); // Convertemos o número criptografado para uma string

        strcat(current_position, encrypted_char); // Concatenamos o número criptografado à string de resultado

        if (i < msg_len - 1)
        {
            strcat(current_position, " "); // Adicionamos espaço entre os números criptografados
        }

        current_position += strlen(encrypted_char); // Movemos o ponteiro de posição para o próximo espaço disponível

        // Devemos nos certificar de não ultrapassar o espaço alocado
        if (current_position - *result >= (msg_len * 20))
        {
            fprintf(stderr, "Erro: Espaço alocado insuficiente.\n");
            exit(1);
        }
    }

    mpz_clears(msg, e, n, encrypted, NULL); // Liberamos a memória usada pelos números mpz_t
}

/* EMSCRIPTEN_KEEPALIVE */
char *cryptosia_encrypt(const char *message, const char *e_string, const char *n_string)
{
    char *result = NULL;

    printf("Mensagem Original: %s\n", message);
    printf("Chave Pública: e = %s, n = %s\n", e_string, n_string);

    RSA_encrypt(message, e_string, n_string, &result);

    printf("Mensagem Criptografada: %s\n", result);

    return result;
}

// ======================= DESENCRIPTAÇÃO DE MENSAGENS  ====================================

/*
    CONCEITO MATEMÁTICO
    Para desencriptar a mensagem, calculamos o resto da divisão de c elevado a d por n.
    Ou seja, m = (c ^ d) % n
*/

void RSA_decrypt(const char *encrypted_message, const char *d_str, const char *n_str, char **result)
{
    mpz_t encrypted, d, n, decrypted;
    mpz_inits(encrypted, d, n, decrypted, NULL);

    // Configuramos a chave pública, expressa em valores mpz_t (strings de números bem grandes), para a base decimal (10)
    mpz_set_str(d, d_str, 10);
    mpz_set_str(n, n_str, 10);

    size_t encrypted_len = strlen(encrypted_message); // Obtemos o tamanho da mensagem criptografada
    *result = (char *)malloc(encrypted_len * 2);      // Alocamos memória para armazenar a mensagem descriptografada (2 é um tamanho aproximado)

    if (*result == NULL)
    {
        fprintf(stderr, "Erro na alocação de memória.\n");
        exit(1);
    }

    char *current_position = *result;                     // Criamos um ponto de partida para escrever na memória alocada
    char *token = strtok((char *)encrypted_message, " "); // Dividimos a string criptografada por espaços

    while (token != NULL)
    {
        mpz_set_str(encrypted, token, 10); // Convertemos o token (número criptografado) para mpz_t

        // ========

        // Realizamos a descriptografia RSA encontrando o equivalente decifrado "M" a partir da seguinte função:
        // M = (c ^ d) % n (c = número criptografado, d = expoente, n = produto de p e q)

        // Verificamos se os valores são grandes demais para serem manipulados pela nossa função de exponenciação modular rápida
        mpz_t multiplication;
        mpz_init(multiplication);

        mpz_mul(multiplication, n, n);

        // Caso seja possível, utilizamos nossa função própria de exponenciação modular rápida
        if (mpz_fits_ulong_p(multiplication))
        {
            printf("Fits ulong\n");
            unsigned long long int encrypted_int = mpz_get_ui(encrypted);
            unsigned long long int d_int = mpz_get_ui(d);
            unsigned long long int n_int = mpz_get_ui(n);

            unsigned long long int int_result = modular_pow(encrypted_int, d_int, n_int);

            // E convertemos o resultado de volta para um mpz_t, que será atribuido à variável "decrypted".
            mpz_set_ui(decrypted, int_result);
        }
        else
        {
            // Caso não seja possível, utilizamos a função mpz_powm
            mpz_powm(decrypted, encrypted, d, n);
        }

        mpz_clear(multiplication); // Liberamos a memória usada pelo número mpz_t

        // ========

        unsigned long long int ascii_value = mpz_get_ui(decrypted); // Obtemos o valor ASCII descriptografado

        char decrypted_char[2] = {(char)ascii_value, '\0'}; // Convertemos o valor ASCII de volta para caractere
        strcat(current_position, decrypted_char);           // Concatenamos o caractere à string de resultado

        current_position += strlen(decrypted_char); // Move o ponteiro de posição para o próximo espaço disponível

        // Devemos nos certificar de não ultrapassar o espaço alocado
        if (current_position - *result >= (encrypted_len * 2))
        {
            fprintf(stderr, "Erro: Espaço alocado insuficiente.\n");
            exit(1);
        }

        token = strtok(NULL, " "); // Passamos para o próximo token (número criptografado)
    }

    mpz_clears(encrypted, d, n, decrypted, NULL); // Liberamos a memória usada pelos números mpz_t
}

/* EMSCRIPTEN_KEEPALIVE */
char *cryptosia_decrypt(const char *encrypted_message, const char *d_string, const char *n_string)
{
    char *result = NULL;
    RSA_decrypt(encrypted_message, d_string, n_string, &result);

    printf("Mensagem Descriptografada: %s\n", result);

    return result;
}

/*
    PARA TESTES:
        p: 42292427
        q: 35526487

        Chave Pública {
            e = 364822070128145
            n = 1502501358013949
        }

        Chave Privada: {
            d = 558181515424889
            n = 1502501358013949
        }

    const char *e_str = "364822070128145"; // valor de e
    const char *d_str = "558181515424889"; // valor de d
    const char *n_str = "1502501358013949";// valor de n

    CHAVES MENORES:
    const char *e_str = "65537";
    const char *d_str = "44273";
    const char *n_str = "49163";
*/

/* int main()
{
    const char *message = "A acentuação gráfica consiste na colocação de acento ortográfico para indicar a pronúncia de uma vogal ou marcar a sílaba tônica de uma palavra. Os nomes dos acentos gráficos da língua portuguesa são...";

    const char *e_str = "364822070128145";  // valor de e
    const char *d_str = "558181515424889";  // valor de d
    const char *n_str = "1502501358013949"; // valor de n

    printf("Mensagem Original: %s\n", message);

    char *encrypted_result = NULL;
    RSA_encrypt(message, e_str, n_str, &encrypted_result);
    printf("Mensagem Criptografada: %s\n", encrypted_result);

    char *decrypted_result = NULL;
    RSA_decrypt(encrypted_result, d_str, n_str, &decrypted_result);
    printf("Mensagem Descriptografada: %s\n", decrypted_result);

    free(encrypted_result); // Liberar memória alocada
    free(decrypted_result); // Liberar memória alocada

    return 0;
} */