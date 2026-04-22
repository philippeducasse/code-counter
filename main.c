#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

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

int has_counted_extention(const char *name, const char *extensions[])
{
    const char *ext = strrchr(name, '.');
    if (!ext)
        return 0;

    for (int i = 0; extensions[i] != NULL; i++)
    {
        if (strcmp(ext, extensions[i]) == 0)
            return 1;
    }
    return 0;
}

int is_counted_file(const char *name)
{
    if (name[0] == '.')
        return 0; // skip hidden files

    const char *exts[] = {".py", ".ts", ".tsx", ".js", ".jsx", ".vue", ".yml", ".yaml", ".toml", ".c", NULL};
    return has_counted_extention(name, exts) || strstr(name, "ocker") != NULL;
}

int walk_dir(const char *path)
{
    DIR *dir = opendir(path);

    if (!dir)
    {
        perror("opendir");
        return -1;
    }

    int total = 0;
    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // skip hidden directories
        if (should_skip_dir(entry->d_name))
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && is_counted_file(entry->d_name))
        {
            total += count_lines(full_path);
        }
        else if (S_ISDIR(st.st_mode))
        {
            total += walk_dir(full_path);
        }
    }

    closedir(dir);
    return total;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./cc <path>\n");
        return 1;
    }
    char *path = argv[1];

    int lines = walk_dir(path);
    if (lines == -1)
    {
        perror("main");
        return 1;
    }

    printf("Total lines: %d\n", lines);
    return 0;
}