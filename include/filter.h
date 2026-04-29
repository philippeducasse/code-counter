#ifndef FILTER_H
#define FILTER_H

int should_skip_dir(const char *name, const char **included);
int is_counted_file(const char *name);
int has_counted_extention(const char *name, const char *extensions[]);

#endif