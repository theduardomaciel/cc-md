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

        printf("result: %llu\n", result);

        // Agora, o expoente é sempre par, então podemos dividir a base ao quadrado.
        base = (base * base) % mod;
        exponent = exponent >> 1; // dividimos o valor do expoente pela metade
    }

    return result;
}

// FUNÇÃO ADAPTADA
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

/*
    CONCEITO MATEMÁTICO:
    A mensagem é encriptada em blocos de tamanho fixo, onde cada bloco é um número inteiro entre 0 e n - 1.
    Para encriptar um bloco m, calculamos o resto da divisão de m elevado a e por n.
    Ou seja, c = (m ^ e) % n
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
    mpz_t current_char, e, n, current_encrypted;
    mpz_inits(current_char, e, n, current_encrypted, NULL);

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

        /*
            OBSERVAÇÃO:
            Para suportar acentos latinos e outros caracteres especiais, utilizamos a tabela ASCII estendida.
            Para isso, descartamos a chamada de "mpz_set_ui" na linha acima e utilizamos o seguinte:
                unsigned char current_char = (unsigned char)(message[i]);
                mpz_set_ui(msg, current_char);
         */

        // Realizamos a criptografia encontrando o equivalente cifrado "C" a partir da seguinte função:
        // C = (m ^ e) % n (m = caractere atual convertido em numeral ASCII, e = expoente, n = produto de p e q)

        // Verificamos se os valores são grandes demais para serem manipulados pela nossa função de exponenciação modular rápida
        mpz_t multiplication;
        mpz_init(multiplication);

        mpz_mul(multiplication, n, n);

        // Caso seja possível, utilizamos nossa função própria de exponenciação modular rápida
        if (mpz_fits_ulong_p(multiplication))
        {
            unsigned long long int current_char_int = mpz_get_ui(current_char);
            unsigned long long int e_int = mpz_get_ui(e);
            unsigned long long int n_int = mpz_get_ui(n);

            unsigned long long int int_result = modular_pow(current_char_int, e_int, n_int);

            // E convertemos o resultado de volta para um mpz_t, que será atribuido à variável "current_encrypted".
            mpz_set_ui(current_encrypted, int_result);
        }
        else
        {
            // Caso não seja possível, utilizamos a função mpz_powm
            mpz_powm(current_encrypted, current_char, e, n);
        }

        // Convertemos o valor de "current_encrypted" para uma string (visto que o número pode ser bem grande)...
        char encrypted_char[1024];
        mpz_get_str(encrypted_char, 10, current_encrypted);

        strcat(result, encrypted_char); // ...e concatenamos o resultado com a variável "result"

        if (i < message_length - 1)
        {
            strcat(result, " "); // Adicionamos espaço ENTRE os números
        }
    }

    mpz_clears(current_char, e, n, current_encrypted, NULL);
}

// ======================= DESENCRIPTAÇÃO DE MENSAGENS  ====================================

/*
    CONCEITO MATEMÁTICO
    Para desencriptar a mensagem, calculamos o resto da divisão de c elevado a d por n.
    Ou seja, m = (c ^ d) % n
*/

void RSA_decrypt(const char *encrypted_message, const char *d_string, const char *n_string, char *result)
{
    mpz_t encrypted, d, n, decrypted;
    mpz_inits(encrypted, d, n, decrypted, NULL);

    mpz_set_str(d, d_string, 10);
    mpz_set_str(n, n_string, 10);

    char *token = strtok((char *)encrypted_message, " "); // Divide a string por espaços

    while (token != NULL)
    {
        mpz_set_str(encrypted, token, 10); // Convertemos o valor para a base decimal (10)

        // Realizamos a criptografia inversa para encontrar o caractere original (m) por meio da seguinte fórmula:
        // m = (c ^ d) % n (c = caractere cifrado, d = chave privada, n = produto de p e q)

        // Verificamos se os valores são grandes demais para serem manipulados pela nossa função de exponenciação modular rápida
        mpz_t multiplication;
        mpz_init(multiplication);

        mpz_mul(multiplication, n, n);

        // Caso seja possível, utilizamos nossa função própria de exponenciação modular rápida
        if (mpz_fits_ulong_p(multiplication))
        {
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

        // Convertemos o valor de "decrypted" de mpz_t para um inteiro...
        unsigned long long int ascii_value = mpz_get_ui(decrypted);

        char decrypted_char[2] = {(char)ascii_value, '\0'}; // ...e convertemos o inteiro para um caractere
        strcat(result, decrypted_char);

        token = strtok(NULL, " "); // Próximo token
    }

    mpz_clears(encrypted, d, n, decrypted, NULL);
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

*/

/* int main()
{
    const char *message = "Agora, mais difícil! :)"; // mensagem original
    const char *e_str = "364822070128145";           // valor de e
    const char *d_str = "558181515424889";           // valor de d
    const char *n_str = "1502501358013949";          // valor de n

    printf("Mensagem Original: %s\n", message);

    char encrypted_result[1024] = {0};
    RSA_encrypt(message, e_str, n_str, encrypted_result);
    printf("Mensagem Criptografada: %s\n", encrypted_result);

    char decrypted_result[1024] = {0};
    RSA_decrypt(encrypted_result, d_str, n_str, decrypted_result);
    printf("Mensagem Descriptografada: %s\n", decrypted_result);

    return 0;
} */