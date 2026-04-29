#include "walker.h"
#include "filter.h"
#include "counter.h"
#include "config.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

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
            continue;
        if (should_skip_dir(entry->d_name, included))
            continue;

        char full_path[MAX_PATH_LENGTH];
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
    return 0;
}