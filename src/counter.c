#include "counter.h"
#include <stdio.h>

int count_lines(const char *path)
{
    FILE *file = fopen(path, "r");
    if (!file)
    {
        perror("fopen");
        return -1;
    }

    int count = 0;
    char ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
            count++;
    }

    fclose(file);
    return count;
}