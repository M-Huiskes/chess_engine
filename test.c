#include <stdio.h>

int main()
{
    int numbers[] = {10, 20, 30, 40, 50}; // compiler figures out size
    int length = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < length; i++)
    {
        printf("%d\n", numbers[i]);
    }
}