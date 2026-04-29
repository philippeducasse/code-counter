#ifndef WALKER_H
#define WALKER_H

struct LangStats
{
    const char *ext;
    int total_lines;
    int file_count;
};

int walk_dir(const char *path, struct LangStats stats[], const char **included);

#endif