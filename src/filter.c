#include "filter.h"
#include "config.h"
#include <string.h>

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

int should_skip_dir(const char *name, const char **included)
{
    for (int i = 0; SKIP_DIRECTORIES[i] != NULL; i++)
    {
        if (strcmp(name, SKIP_DIRECTORIES[i]) == 0)
        {
            if (included)
            {
                for (int j = 0; included[j] != NULL; j++)
                {
                    if (strcmp(name, included[j]) == 0)
                        return 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

int is_counted_file(const char *name)
{
    if (name[0] == '.')
        return 0;

    return has_counted_extention(name, (const char **)SUPPORTED_EXTENSIONS) || strstr(name, "ocker") != NULL;
}