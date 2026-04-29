#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "walker.h"
#include "config.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: ./loc <path> [--include dir1 dir2 ...]\n");
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

    if (argc > 2 && strcmp(argv[2], "--include") == 0)
    {
        for (int i = 3; i < argc; i++)
        {
            included[included_count] = argv[i];
            included_count++;
        }
    }
    included[included_count] = NULL;

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