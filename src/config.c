#include "config.h"
#include <stddef.h>

const char *SKIP_DIRECTORIES[] = {
    ".git",
    "node_modules",
    "__pycache__",
    "migrations",
    "staticfiles",
    "static",
    "dist",
    "build",
    NULL
};

const char *SUPPORTED_EXTENSIONS[] = {
    ".py",
    ".ts",
    ".tsx",
    ".js",
    ".jsx",
    ".vue",
    ".yml",
    ".yaml",
    ".toml",
    ".c",
    NULL
};