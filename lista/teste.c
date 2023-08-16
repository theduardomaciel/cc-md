#include <stdio.h>

// MDC functions
int lista_para_mdc(int num, int fator, int array[])
{
    int index = 0;
    while (num != 1)
    {
        if (num % fator == 0)
        {
            num = num / fator;
            if (array[fator] == 0)
            {
                array[fator] = fator;
                index++;
            }
        }
        else
        {
            fator++;
        }

        if (fator >= 100)
        {
            break; // Safety measure to prevent infinite loop
        }
    }
    return fator;
}

// MMC functions
int lista_para_mmc(int num, int fator, int array[], int *potencia)
{
    while (num != 1)
    {
        if (num % fator == 0)
        {
            num = num / fator;
            (*potencia)++;
            if (num == 1)
            {
                if (array[fator] == 0)
                {
                    array[fator] = fator * (*potencia);
                }
            }
        }
        else
        {
            if (array[fator] == 0)
            {
                array[fator] = fator * (*potencia);
            }
            *potencia = 0;
            fator++;
        }
    }
    return fator;
}

void build_mmc(int array1[], int array2[], int mmc[]);

void build_mmc(int array1[], int array2[], int mmc[])
{
    int mmc_index = 0;
    for (int i = 0; array1[i] != 0; i++)
    {
        for (int j = 0; array2[j] != 0; j++)
        {
            if (array1[i] % array2[j] != 0)
            {
                int is_divisor = 0;
                for (int k = 0; k < mmc_index; k++)
                {
                    if (mmc[k] == array2[j])
                    {
                        is_divisor = 1;
                        break;
                    }
                }
                if (!is_divisor)
                {
                    mmc[mmc_index] = array2[j];
                    mmc_index++;
                }
            }
            if (array2[j] % array1[i] != 0)
            {
                int is_divisor = 0;
                for (int k = 0; k < mmc_index; k++)
                {
                    if (mmc[k] == array1[i])
                    {
                        is_divisor = 1;
                        break;
                    }
                }
                if (!is_divisor)
                {
                    mmc[mmc_index] = array1[i];
                    mmc_index++;
                }
            }
        }
    }
    // Sort MMC array
    for (int i = 0; i < mmc_index; i++)
    {
        for (int j = i + 1; j < mmc_index; j++)
        {
            if (mmc[i] > mmc[j])
            {
                int temp = mmc[i];
                mmc[i] = mmc[j];
                mmc[j] = temp;
            }
        }
    }
    // Remove common divisors
    int mmc_size = mmc_index;
    for (int i = 0; i < mmc_size; i++)
    {
        int divisor = mmc[i];
        for (int j = i + 1; j < mmc_size; j++)
        {
            if (mmc[j] % divisor == 0)
            {
                for (int k = j; k < mmc_size - 1; k++)
                {
                    mmc[k] = mmc[k + 1];
                }
                mmc_size--;
                j--;
            }
        }
    }
}

int main()
{
    printf("\nCalculadora de mdc e mmc entre dois números");
    int n, p;
    printf("\ninsira o primeiro valor: ");
    scanf("%d", &n);
    printf("insira o segundo valor: ");
    scanf("%d", &p);
    int aux1 = n;
    int aux2 = p;
    int array_n[100] = {0};
    int array_p[100] = {0};
    int mdc[100] = {0};
    int mmc[100] = {0};

    int array_exponent[100] = {1};
    int array_exponent2[100] = {1};

    lista_para_mdc(n, 2, array_n);
    lista_para_mdc(p, 2, array_p);

    int mdc_index = 0;
    for (int j = 0; j < 100; j++)
    {
        if (array_n[j] == array_p[j] && array_n[j] != 0)
        {
            mdc[mdc_index] = array_n[j];
            mdc_index++;
        }
    }

    int potencia_n = 0;
    int potencia_p = 0;
    lista_para_mmc(n, 2, array_exponent, &potencia_n);
    lista_para_mmc(p, 2, array_exponent2, &potencia_p);

    int mmc_index = 0;
    for (int n_index = 0; n_index < 100; n_index++)
    {
        for (int p_index = 0; p_index < 100; p_index++)
        {
            if ((array_exponent[n_index] % array_exponent2[p_index] != 0) && array_exponent2[p_index] != 0)
            {
                mmc[mmc_index] = array_exponent2[p_index];
                mmc_index++;
            }
            if ((array_exponent2[p_index] % array_exponent[n_index] != 0) && array_exponent[n_index] != 0)
            {
                mmc[mmc_index] = array_exponent[n_index];
                mmc_index++;
            }
        }
    }

    build_mmc(array_exponent, array_exponent2, mmc);

    printf("\nresultados:\n");
    printf("mdc(%d,%d) =", aux1, aux2);
    int mdc_product = 1;
    for (int i = 0; mdc[i] != 0; i++)
    {
        printf(" %d", mdc[i]);
        mdc_product *= mdc[i];
    }
    printf("\nmmc(%d,%d) =", aux1, aux2);
    int mmc_product = 1;
    for (int i = 0; mmc[i] != 0; i++)
    {
        printf(" %d", mmc[i]);
        mmc_product *= mmc[i];
    }
    printf("\n");

    return 0;
}
