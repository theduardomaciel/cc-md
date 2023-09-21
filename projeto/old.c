#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void menuPublicKey();
void menuEncrypt();
void menuDecrypt();
void decryptMessage(int firstPrime, int secondPrime, int exponent, char *message);
void encryptMessage(char *message, int publicKey, int exponent);
void createPublicKey(int firstPrime, int secondPrime, int exponent);
int mdc(int a, int b);
int isPrime(int n);
void generateExponent(int totient);

void menuPublicKey()
{
    printf("----------- Criptografia RSA -----------\n");
    printf("Gerar chave pública\n");
    int first_prime = 0;
    int second_prime = 0;
    while (first_prime * second_prime < 256)
    {
        printf("Informe o primeiro número primo: ");
        scanf("%d", &first_prime);

        while (!isPrime(first_prime))
        {
            printf("O número deve ser primo, informe novamente um número: ");
            scanf("%d", &first_prime);
        }
        printf("Informe o segundo número primo: ");
        scanf("%d", &second_prime);
        while (!isPrime(second_prime))
        {
            printf("O número deve ser primo, informe novamente um número: ");
            scanf("%d", &second_prime);
        }

        if (first_prime * second_prime < 256)
        {
            printf("O produto dos números primos deve ser maior que 256. Digite novamente os números primos.\n");
        }
    }
    int totient = (first_prime - 1) * (second_prime - 1);
    printf("Sugerimos utilizar algum destes expoentes:");
    generateExponent(totient);
    int exponent;
    printf("Informe o expoente desejado: ");
    scanf("%d", &exponent);
    while (mdc(exponent, totient) != 1)
    {
        printf("O expoente deve ser relativamente primo ao produto dos números primos menos 1. Informe novamente o expoente: ");
        scanf("%d", &exponent);
    }
    createPublicKey(first_prime, second_prime, exponent);
}

void menuEncrypt()
{
    printf("----------- Criptografia RSA -----------\n");
    printf("Encriptar Mensagem\n");
    char message[255];
    printf("Informe a mensagem que você deseja encriptar: ");
    scanf("%s", message);
    int public_key;
    printf("Informe a chave pública: ");
    scanf("%d", &public_key);
    int exponent;
    printf("Informe o expoente utilizado: ");
    scanf("%d", &exponent);
    encryptMessage(message, public_key, exponent);
}

void menuDecrypt()
{
    printf("----------- Criptografia RSA -----------\n");
    printf("Descriptar Mensagem\n");
    int first_prime;
    printf("Informe o primeiro número primo utilizado: ");
    scanf("%d", &first_prime);
    int second_prime;
    printf("Informe o segundo número primo utilizado: ");
    scanf("%d", &second_prime);
    int exponent;
    printf("Informe o expoente utilizado: ");
    scanf("%d", &exponent);
    char message[255];
    printf("Informe a mensagem encriptada: ");
    scanf("%s", message);
    decryptMessage(first_prime, second_prime, exponent, message);
}

void decryptMessage(int firstPrime, int secondPrime, int exponent, char *message)
{
    int D = 2;
    int totient = (firstPrime - 1) * (secondPrime - 1);
    int N = firstPrime * secondPrime;
    while ((D * exponent) % totient != 1)
    {
        D++;
    }
    FILE *decrypt = fopen("decrypted.txt", "w");
    char *token = strtok(message, " ");
    while (token != NULL)
    {
        int num = atoi(token);
        int t = (int)pow(num, D) % N;
        fputc((char)t, decrypt);
        token = strtok(NULL, " ");
    }
    fclose(decrypt);
    printf("O arquivo foi gerado com sucesso!\n");
    char saida[255];
    printf("Pressione ENTER para continuar...");
    fgets(saida, sizeof(saida), stdin);
}

void encryptMessage(char *message, int publicKey, int exponent)
{
    FILE *file = fopen("encrypted.txt", "w");
    for (int i = 0; i < strlen(message); i++)
    {
        int value = (int)pow(message[i], exponent) % publicKey;
        fprintf(file, "%d ", value);
    }
    fclose(file);
    printf("O arquivo com a mensagem criptografada foi gerado com sucesso!\n");
    char saida[255];
    printf("Pressione ENTER para continuar...");
    fgets(saida, sizeof(saida), stdin);
}

void createPublicKey(int firstPrime, int secondPrime, int exponent)
{
    int public_key = firstPrime * secondPrime;
    FILE *file = fopen("public_key.txt", "w");
    FILE *private = fopen("private_key.txt", "w");
    fprintf(file, "%d %d", public_key, exponent);
    fprintf(private, "%d %d", firstPrime, secondPrime);
    if (file != NULL)
    {
        printf("Arquivo criado com sucesso! Sua chave pública gerada é %d e o expoente utilizado foi %d.\n", public_key, exponent);
    }
    else
    {
        printf("Não foi possível criar o arquivo\n");
    }
    fclose(file);
    fclose(private);
    char saida[255];
    printf("Pressione ENTER para continuar...");
    fgets(saida, sizeof(saida), stdin);
}

int mdc(int a, int b)
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

int isPrime(int n)
{
    if (n == 2 || n == 3)
    {
        return 1;
    }
    for (int i = 2; i <= sqrt(n); i++)
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

void generateExponent(int totient)
{
    int quantity = 10;
    int exponent = 2;
    printf(" (");
    while (quantity)
    {
        while (mdc(exponent, totient) != 1)
        {
            exponent++;
        }
        printf("%d", exponent);
        exponent++;
        if (quantity != 1)
        {
            printf(",");
        }
        else
        {
            printf(")\n");
        }
        quantity--;
    }
}

int main()
{
    int user_input;
    do
    {
        printf("----------- Criptografia RSA -----------\n\n");
        printf("1. Gerar chave pública\n");
        printf("2. Encriptar\n");
        printf("3. Desencriptar\n");
        printf("4. Encerrar\n");
        scanf("%d", &user_input);
        switch (user_input)
        {
        case 1:
            menuPublicKey();
            break;
        case 2:
            menuEncrypt();
            break;
        case 3:
            menuDecrypt();
            break;
        case 4:
            break;
        default:
            printf("Informe uma opção válida\n Digite novamente a opção:\n");
            break;
        }
    } while (user_input != 4);
    return 0;
}