#include <stdio.h>
#include <string.h>

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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./cc <path>\n");
        return 1;
    }

    char *path = argv[1];
    int lines = count_lines(path);

    printf("Total lines: %d\n", lines);
    return 0;
}

int should_skip_dir(const char *name)
{
    const char *skip[] = {".git",
                          "node_modules",
                          "__pycache__",
                          "migrations",
                          NULL};

    for (int i = 0; skip[i] != NULL; i++)
    {
        if (strcmp(name, skip[i]) == 0)
            return 1;
    }
    return 0;
}

int is_counted_file(const char *name)
{
    if (name[0] == '.')
        return 0; // skip hidden files

    const char *ext = strrchr(name, '.'); // strrchr finds last occurence of a char and returns a pointer to that occurence
    if (!ext)
        return 0;

    return strcmp(ext, ".py") == 0 || strcmp(ext, ".ts") == 0 || strcmp(ext, ".js") == 0 || strcmp(ext, ".c") == 0;
}