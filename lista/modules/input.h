#include <stdio.h>

int request_input(int amount, int *array_b, int *array_m)
{
    for (int i = 0; i < amount; i++)
    {
        printf("✍️ >> Congruência %d: ", i + 1);

        int b, m;
        int scan_return = scanf("%d %d", &b, &m);

        if (scan_return != 2)
        {
            printf("✖️ O input inserido não satisfaz o padrão: b m\n");
            break;
            return 0;
        }

        array_b[i] = b;
        array_m[i] = m;
    }

    return 1;
}