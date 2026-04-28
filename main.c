#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
struct LangStats
{
    const char *ext;
    int total_lines;
    int file_count;
};

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

int should_skip_dir(const char *name, const char **included)
{
    const char *skip[] = {".git",
                          "node_modules",
                          "__pycache__",
                          "migrations",
                          "staticfiles",
                          "static",
                          "dist",
                          "build",
                          NULL};

    for (int i = 0; skip[i] != NULL; i++)
    {
        if (strcmp(name, skip[i]) == 0)
        {
            for (int j = 0; included[j] != NULL; j++)
            {
                if (strcmp(name, included[j]) == 0)
                {
                    return 0;
                }
            }
            return 1;
        }
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

int walk_dir(const char *path, struct LangStats stats[], const char **included)
{
    DIR *dir = opendir(path);

    if (!dir)
    {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // skip hidden directories
        if (should_skip_dir(entry->d_name, included))
            continue;

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1)
            continue;

        if (S_ISREG(st.st_mode) && is_counted_file(entry->d_name))
        {
            int lines = count_lines(full_path);

            for (int i = 0; stats[i].ext != NULL; i++)
            {
                if (has_counted_extention(entry->d_name, (const char *[]){stats[i].ext, NULL}))
                {
                    stats[i].total_lines += lines;
                    stats[i].file_count++;
                    break;
                }
            }
        }
        else if (S_ISDIR(st.st_mode))
        {
            walk_dir(full_path, stats, included);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./cc <path>\n");
        return 1;
    }
    struct LangStats stats[] = {
        {".py", 0, 0},
        {".ts", 0, 0},
        {".tsx", 0, 0},
        {".js", 0, 0},
        {".jsx", 0, 0},
        {".vue", 0, 0},
        {".yml", 0, 0},
        {".yaml", 0, 0},
        {".toml", 0, 0},
        {".c", 0, 0},
        {NULL, 0, 0}};

    char *path = argv[1];

    const char **included = malloc(argc * sizeof(const char *));

    int included_count = 0;

    if (argc > 2)
    {
        if (strcmp(argv[2], "--include") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                included[included_count] = argv[i];
                included_count++;
            }
            included[included_count] = NULL;
        }
    }
    walk_dir(path, stats, included);

    int total_lines = 0;
    int total_files = 0;

    for (int i = 0; stats[i].ext != NULL; i++)
    {
        total_lines += stats[i].total_lines;
        total_files += stats[i].file_count;
    }

    printf("Total lines: %d\n", total_lines);
    printf("Average lines per file: %d\n", total_files > 0 ? total_lines / total_files : 0);
    for (int i = 0; stats[i].ext != NULL; i++)
    {
        if (stats[i].file_count > 0)
        {
            printf("%s files: %d lines (%d files)\n",
                   stats[i].ext,
                   stats[i].total_lines,
                   stats[i].file_count);
        }
    }
    free(included);
    return 0;
}