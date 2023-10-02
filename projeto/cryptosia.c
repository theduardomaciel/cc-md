#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <assert.h>

#include <emscripten.h>
#include "gmp.h"

// ================= FUNÇÕES AUXILIARES ==============================

EMSCRIPTEN_KEEPALIVE
char *mdc(const char *a_str, const char *b_str)
{
    // Inicializamos objetos GMP para os números a e b.
    mpz_t a, b;
    mpz_inits(a, b, NULL);

    // Convertemos as strings em objetos GMP.
    mpz_set_str(a, a_str, 10);
    mpz_set_str(b, b_str, 10);

    // Calculamos o MDC manualmente
    while (mpz_cmp_ui(b, 0) != 0)
    {
        mpz_t temp;
        mpz_init(temp);
        mpz_set(temp, b);
        mpz_mod(b, a, b);
        mpz_set(a, temp);
        mpz_clear(temp);
    }

    // Convertemos o resultado em uma string.
    char *gcd_str = mpz_get_str(NULL, 10, a);

    // Liberamos a memória dos objetos GMP.
    mpz_clears(a, b, NULL);

    // Retornamos a string do MDC.
    return gcd_str;
}

/*
// OUTRA POSSÍVEL IMPLEMENTAÇÃO PARA A FUNÇÃO "mdc"
char *mdc(const char *a_str, const char *b_str)
{
    // Convertemos as strings em números inteiros.
    long long int a = atoll(a_str);
    long long int b = atoll(b_str);

    // Calcula o MDC manualmente.
    while (b != 0)
    {
        long long int temp = b;
        b = a % b;
        a = temp;
    }

    // Convertemos o resultado em uma string.
    char gcd_str[64]; // Tamanho arbitrário para a string.
    snprintf(gcd_str, sizeof(gcd_str), "%lld", a);

    // Alocamos memória e copia o resultado para uma string dinâmica.
    char *result = strdup(gcd_str);

    // Retornamos a string do MDC.
    return result;
}
*/

int isPrime(unsigned long long int n)
{
    if (n == 2 || n == 3)
    {
        return 1;
    }

    for (unsigned long long int i = 2; i <= sqrt(n); i++)
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

/*
    TABELA DE ERROS:
        A = o valor de p ou q ultrapassa os limites da linguagem C
        B = o valor do produto de p e q é menor que 256, ou seja, a criptografia não é suficientemente segura
        C = o valor de p não é primo
        D = o valor de q não é primo
        E = o valor de p ou q não é primo
*/

// A chave pública é constituída por dois elementos: n e e
// O elemento n é o produto de dois números primos p e q
// O elemento e é um expoente co-primo com o totiente de n
// O elemento d é o inverso multiplicativo de e módulo o totiente de n

// O par (e, n) forma a chave pública.
// O par (d, n) forma a chave privada.

/*
    CONCEITO MATEMÁTICO:
    "p" e "q" são dois números primos grandes distintos. Esses números são mantidos em segredo e não são compartilhados.
*/

char *verifications(const char *p_str, const char *q_str)
{
    mpz_t p, q;
    mpz_t p_times_q;
    int result = 1; // Inicializamos com 1 como padrão

    // Inicializamos as variáveis GMP para p, q e o produto de p e q.
    mpz_inits(p, q, p_times_q, NULL);

    mpz_set_str(p, p_str, 10);
    mpz_set_str(q, q_str, 10);

    int is_p_prime = isPrime(mpz_get_ui(p));
    int is_q_prime = isPrime(mpz_get_ui(q));

    if (mpz_cmp_ui(p_times_q, 256) < 0)
    {
        // O valor do produto de p e q é menor que 256, ou seja, a criptografia não é suficientemente segura
        result = -2;
    }
    else if (!is_p_prime && is_q_prime)
    {
        // O valor de p não é primo
        result = -3;
    }
    else if (!is_q_prime && is_p_prime)
    {
        // O valor de q não é primo
        result = -4;
    }
    else if (!is_p_prime && !is_q_prime)
    {
        // O valor de p ou q não é primo
        result = -5;
    }

    // Liberamos a memória dos objetos GMP.
    mpz_clears(p, q, p_times_q, NULL);

    char *result_str = malloc(2);          // Alocamos espaço para um inteiro com até 2 dígitos
    snprintf(result_str, 2, "%d", result); // Convertemos o resultado para uma string

    return result_str;
}

/*
    CONCEITO MATEMÁTICO:
    "n" é o produto de dois números primos p e q.
*/

// ELEMENTO DA CHAVE PÚBLICA E PRIVADA
EMSCRIPTEN_KEEPALIVE
char *n_factor(const char *p_str, const char *q_str)
{
    // Verificamos se há alguma inconsistência nos valores de p e q.
    char *verificationsResult = verifications(p_str, q_str);

    if (verificationsResult[0] == '-')
    {
        // Inicializamos as variáveis GMP para p, q e n.
        mpz_t p, q, n;
        mpz_init(p);
        mpz_init(q);
        mpz_init(n);

        // Convertemos as strings de p e q para objetos GMP.
        mpz_set_str(p, p_str, 10);
        mpz_set_str(q, q_str, 10);

        // Calculamos o produto de p e q.
        mpz_mul(n, p, q);

        // Convertemos o produto para uma string.
        char *result_str = mpz_get_str(NULL, 10, n);

        // Liberamos a memória dos objetos GMP.
        mpz_clears(p, q, n, NULL);

        free(verificationsResult); // Liberaros a memória alocada para verificationsResult

        return result_str;
    }
    else
    {
        return verificationsResult;
    }
}

/*
    CONCEITO MATEMÁTICO:
    O totiente de um número inteiro positivo n é denotado como φ(n)
    "Por exemplo, se n for um número primo, então φ(n) será igual a n - 1, pois todos os inteiros positivos menores que n são coprimos com n.
    Por outro lado, se n for o produto de dois números primos distintos, como n = p * q, onde p e q são primos diferentes,
    então φ(n) será igual a (p - 1) * (q - 1), porque apenas os múltiplos de p, os múltiplos de q e seus múltiplos comuns não são coprimos com n."

    Portanto, para calcular a chave privada utilizamos a função totiente de Euler calculando: φ(n) = (p - 1) * (q - 1)
*/

EMSCRIPTEN_KEEPALIVE
char *publicKey_totient(const char *p_str, const char *q_str)
{
    // Inicializamos as variáveis GMP para p, q e o totiente.
    mpz_t p, q, totient;
    mpz_inits(p, q, totient, NULL);

    mpz_set_str(p, p_str, 10);
    mpz_set_str(q, q_str, 10);

    // Verificamos se há alguma inconsistência nos valores de p e q.
    char *verificationsResult = verifications(p_str, q_str);

    if (verificationsResult[0] == '-')
    {
        // Subtraímos 1 de p e q e multiplicamos os resultados.
        mpz_sub_ui(p, p, 1);
        mpz_sub_ui(q, q, 1);
        mpz_mul(totient, p, q);

        // Convertemos o totiente para uma string.
        char *totient_str = mpz_get_str(NULL, 10, totient);

        // Liberamos a memória dos objetos GMP.
        mpz_clears(p, q, totient, NULL);
        free(verificationsResult);

        return totient_str;
    }
    else
    {
        mpz_clears(p, q, totient, NULL);
        return verificationsResult;
    }
}

/*
    CONCEITO MATEMÁTICO:
    Escolhemos um número inteiro positivo e relativamente primo (co-primo) a φ(n), geralmente chamado de "e" (exponente de encriptação)
    O valor de e deve ser menor do que φ(n) e maior do que 1.
*/

EMSCRIPTEN_KEEPALIVE
// Função para encontrar os n primeiros expoentes coprimos em relação ao totiente dado como string.
// Recebe o totiente como um char* e retorna uma string contendo os expoentes separados por espaços.
char *publicKey_e(const char *totient_str, int n)
{
    // Inicializamos o objeto GMP para o totiente e o expoente.
    mpz_t totient, exponent;
    mpz_inits(totient, exponent, NULL);

    // Convertemos a string do totiente para um objeto mpz_t.
    mpz_set_str(totient, totient_str, 10);

    // Inicializamos o expoente com o valor 2.
    mpz_set_ui(exponent, 2);

    int count = 0;                      // Criamos um contador para o número de expoentes coprimos encontrados.
    char *coprime_exponents_str = NULL; // Criamos uma string para armazenar os expoentes coprimos encontrados.
    size_t len = 0;                     // Declaramos o tamanho inicial da string.

    // Loopamos até encontrar os n primeiros expoentes coprimos.
    while (count < n)
    {
        // Calculamos o MDC entre o expoente e o totiente como strings.
        char *mdc_result = mdc(mpz_get_str(NULL, 10, exponent), totient_str);

        // Então, se o MDC for igual a "1", o expoente é coprimo.
        if (strcmp(mdc_result, "1") == 0)
        {
            // Como o expoente é coprimo, alocamos memória suficiente para acomodar o novo expoente.
            char *temp = coprime_exponents_str;
            size_t new_len = len + strlen(mpz_get_str(NULL, 10, exponent)) + (len == 0 ? 0 : 1); // +1 para o espaço, se não for o primeiro.
            coprime_exponents_str = (char *)malloc(new_len + 1);                                 // +1 para o caractere nulo.

            // Concatenamos o novo expoente à string de resultados.
            if (len > 0)
            {
                snprintf(coprime_exponents_str, new_len + 1, "%s %s", temp, mpz_get_str(NULL, 10, exponent));
                free(temp);
            }
            else
            {
                snprintf(coprime_exponents_str, new_len + 1, "%s", mpz_get_str(NULL, 10, exponent));
            }

            len = new_len;
            count++;
        }

        // Incrementamos o expoente
        mpz_add_ui(exponent, exponent, 1);
        free(mdc_result);
    }

    // Liberamos a memória dos objetos GMP.
    mpz_clears(totient, exponent, NULL);

    // Retornamos a string contendo os expoentes coprimos separados por espaços.
    return coprime_exponents_str;
}

/*
    CONCEITO MATEMÁTICO:
    Calculamos a chave privada d, que é o inverso multiplicativo de e modulo φ(n).
    Isso significa encontrar um número d tal que (d * e) % φ(n) = 1
*/

EMSCRIPTEN_KEEPALIVE
char *privateKey_d(const char *totient_str, const char *exponent_str)
{
    // Inicializamos os objetos GMP para o totiente, o expoente e a chave privada.
    mpz_t totient, exponent, d, temp;
    mpz_inits(totient, exponent, d, temp, NULL);

    mpz_set_str(totient, totient_str, 10);
    mpz_set_str(exponent, exponent_str, 10);

    mpz_set_ui(d, 1);

    // Calculamos o inverso multiplicativo de e modulo φ(n).
    while (1)
    {
        mpz_mul(temp, d, exponent);   // temp = d * e
        mpz_mod(temp, temp, totient); // temp = (d * e) % φ(n)

        // Se o resultado for igual a 1, encontramos o inverso multiplicativo de e modulo φ(n).
        if (mpz_cmp_ui(temp, 1) == 0)
        {
            break;
        }

        // Se não, incrementamos o valor de d e tentamos novamente.
        mpz_add_ui(d, d, 1);
    }

    // Convertemos o resultado para uma string.
    char *result_str = mpz_get_str(NULL, 10, d);

    // Liberamos a memória dos objetos GMP.
    mpz_clears(totient, exponent, d, temp, NULL);

    return result_str;
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

void RSA_encrypt(const char *message, const char *e_string, const char *n_string, int limit_to_ascii, char **result)
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
        // Para limitar os caracteres à tabela ASCII, utilizamos o seguinte
        if (limit_to_ascii)
        {
            // Traduzimos o valor do caractere em um inteiro obtendo o correspondente do caractere em numeral ASCII
            long int ascii_value = (long int)(message[i]);

            // Alteramos o valor de "current_char" (em mpz_t) para o valor do caractere em numeral ASCII
            mpz_set_ui(msg, ascii_value);
        }
        else
        {
            unsigned char current_char = (unsigned char)(message[i]); // Obtemos o caractere atual
            mpz_set_ui(msg, current_char);                            // Configuramos o número mpz_t com o valor numérico do caractere
        }

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

EMSCRIPTEN_KEEPALIVE
char *cryptosia_encrypt(const char *message, const char *e_string, const char *n_string, int limit_to_ascii)
{
    char *result = NULL;

    printf("Mensagem Original: %s\n", message);
    printf("Chave Pública: e = %s, n = %s\n", e_string, n_string);

    RSA_encrypt(message, e_string, n_string, limit_to_ascii, &result);

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
    size_t result_size = encrypted_len * 2;           // Alocamos memória para armazenar a mensagem descriptografada (2 é um tamanho inicial aproximado)
    *result = (char *)malloc(result_size);

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
            // printf("Fits ulong\n");
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
        if (current_position - *result + strlen(decrypted_char) >= result_size)
        {
            // Realloca mais memória (por exemplo, dobrando o tamanho atual)
            result_size *= 2;
            *result = (char *)realloc(*result, result_size);

            if (*result == NULL)
            {
                fprintf(stderr, "Erro na alocação de memória.\n");
                exit(1);
            }

            current_position = *result + strlen(*result); // Atualiza a posição
        }

        token = strtok(NULL, " "); // Passamos para o próximo token (número criptografado)
    }

    mpz_clears(encrypted, d, n, decrypted, NULL); // Liberamos a memória usada pelos números mpz_t
}

EMSCRIPTEN_KEEPALIVE
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

/*
// TESTE 3
int main()
{
    const char *message = "Testando as coisas com vários sinais estranhões, e pífios novamente!";

    const char *e_str = "65537";   // valor de e
    const char *d_str = "605681";  // valor de d
    const char *n_str = "1469959"; // valor de n

    printf("Mensagem Original: %s\n", message);

    char *encrypted_result = NULL;
    RSA_encrypt(message, e_str, n_str, 0, &encrypted_result);
    printf("Mensagem Criptografada: %s\n", encrypted_result);

    char *decrypted_result = NULL;
    RSA_decrypt(encrypted_result, d_str, n_str, &decrypted_result);
    printf("Mensagem Descriptografada: %s\n", decrypted_result);

    free(encrypted_result); // Liberar memória alocada
    free(decrypted_result); // Liberar memória alocada

    return 0;
} */

/*
// TESTE 2
int main()
{
    const char *message = "Ouviram do Ipiranga as margens plácidas\nDe um povo heroico, o brado retumbante\nE o Sol da liberdade, em raios fúlgidos\nBrilhou no céu da pátria nesse instante\n\nSe o penhor dessa igualdade\nConseguimos conquistar com braço forte\nEm teu seio, ó liberdade\nDesafia o nosso peito a própria morte\n\nÓ Pátria amada\nIdolatrada\nSalve! Salve!\n\nBrasil, um sonho intenso, um raio vívido\nDe amor e de esperança, à terra desce\nSe em teu formoso céu, risonho e límpido\nA imagem do Cruzeiro resplandece\n\nGigante pela própria natureza\nÉs belo, és forte, impávido colosso\nE o teu futuro espelha essa grandeza\n\nTerra adorada\nEntre outras mil\nÉs tu, Brasil\nÓ Pátria amada!\nDos filhos deste solo, és mãe gentil\nPátria amada, Brasil!\n\nDeitado eternamente em berço esplêndido\nAo som do mar e à luz do céu profundo\nFulguras, ó Brasil, florão da América\nIluminado ao Sol do Novo Mundo!\n\nDo que a terra mais garrida\nTeus risonhos, lindos campos têm mais flores\nNossos bosques têm mais vida\nNossa vida, no teu seio, mais amores\n\nÓ Pátria amada\nIdolatrada\nSalve! Salve!\n\nBrasil, de amor eterno seja símbolo\nO lábaro que ostentas estrelado\nE diga o verde-louro dessa flâmula\nPaz no futuro e glória no passado\n\nMas se ergues da justiça a clava forte\nVerás que um filho teu não foge à luta\nNem teme, quem te adora, a própria morte\n\nTerra adorada\nEntre outras mil\nÉs tu, Brasil\nÓ Pátria amada!\nDos filhos deste solo, és mãe gentil\nPátria amada, Brasil!";
    // printf("Mensagem Original: %s\n", message);

    const char *p_str = "1033"; // valor de p
    const char *q_str = "1423"; // valor de q
    // const char *e_str = "65537"; // valor de e

    char *n_str = n_factor(p_str, q_str);
    char *totient_str = publicKey_totient(p_str, q_str);

    char *e_str = publicKey_e(totient_str, 5);
    printf("Chave Pública: e = %s, n = %s\n", e_str, n_str);

    char *d_str = privateKey_d(totient_str, e_str);

    printf("Chave Pública: e = %s, n = %s\n", e_str, n_str);
    printf("Chave Privada: d = %s, n = %s\n", d_str, n_str);

    char *encrypted_result = NULL;
    RSA_encrypt(message, e_str, n_str, &encrypted_result);
    printf("Mensagem Criptografada: %s\n", encrypted_result);
} */

/*
// TESTE 1
int main()
{
    const char *message = "Testando as coisas novamente e novamente!";

    const char *e_str = "65537";   // valor de e
    const char *d_str = "605681";  // valor de d
    const char *n_str = "1469959"; // valor de n

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