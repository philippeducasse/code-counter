#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "counter.h"
#include "filter.h"
#include "config.h"

int tests_passed = 0;
int tests_failed = 0;

void assert_true(int condition, const char *test_name)
{
    if (condition)
    {
        printf("✓ %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf("✗ %s\n", test_name);
        tests_failed++;
    }
}

void assert_equal_int(int actual, int expected, const char *test_name)
{
    if (actual == expected)
    {
        printf("✓ %s\n", test_name);
        tests_passed++;
    }
    else
    {
        printf("✗ %s (expected %d, got %d)\n", test_name, expected, actual);
        tests_failed++;
    }
}

void test_has_counted_extension()
{
    printf("\n--- Testing has_counted_extention ---\n");

    assert_true(has_counted_extention("file.py", (const char *[]){".py", NULL}),
                "Should recognize .py files");
    assert_true(has_counted_extention("file.ts", (const char *[]){".ts", NULL}),
                "Should recognize .ts files");
    assert_true(has_counted_extention("file.jsx", (const char *[]){".jsx", NULL}),
                "Should recognize .jsx files");
    assert_true(!has_counted_extention("file.txt", (const char *[]){".py", NULL}),
                "Should reject .txt files");
    assert_true(!has_counted_extention("noextension", (const char *[]){".py", NULL}),
                "Should reject files without extension");
}

void test_is_counted_file()
{
    printf("\n--- Testing is_counted_file ---\n");

    assert_true(is_counted_file("app.py"),
                "Should count .py files");
    assert_true(is_counted_file("component.tsx"),
                "Should count .tsx files");
    assert_true(is_counted_file("Dockerfile"),
                "Should count Dockerfile");
    assert_true(is_counted_file("docker-compose.yml"),
                "Should count docker-compose files");
    assert_true(!is_counted_file(".hidden.py"),
                "Should skip hidden files");
    assert_true(!is_counted_file("README.txt"),
                "Should skip unsupported extensions");
}

void test_should_skip_dir()
{
    printf("\n--- Testing should_skip_dir ---\n");

    assert_true(should_skip_dir("node_modules", NULL),
                "Should skip node_modules");
    assert_true(should_skip_dir(".git", NULL),
                "Should skip .git");
    assert_true(should_skip_dir("__pycache__", NULL),
                "Should skip __pycache__");
    assert_true(!should_skip_dir("src", NULL),
                "Should not skip src directory");
    assert_true(!should_skip_dir("app", NULL),
                "Should not skip app directory");

    const char *included[] = {"node_modules", NULL};
    assert_true(!should_skip_dir("node_modules", included),
                "Should not skip included directories");
}

void test_count_lines()
{
    printf("\n--- Testing count_lines ---\n");

    FILE *test_file = fopen("/tmp/test_count.txt", "w");
    fprintf(test_file, "line 1\nline 2\nline 3\n");
    fclose(test_file);

    assert_equal_int(count_lines("/tmp/test_count.txt"), 3,
                     "Should count 3 lines");

    FILE *test_file2 = fopen("/tmp/test_empty.txt", "w");
    fprintf(test_file2, "");
    fclose(test_file2);

    assert_equal_int(count_lines("/tmp/test_empty.txt"), 0,
                     "Should count 0 lines in empty file");

    FILE *test_file3 = fopen("/tmp/test_single.txt", "w");
    fprintf(test_file3, "single line\n");
    fclose(test_file3);

    assert_equal_int(count_lines("/tmp/test_single.txt"), 1,
                     "Should count 1 line");
}

void test_config()
{
    printf("\n--- Testing config ---\n");

    assert_true(SKIP_DIRECTORIES[0] != NULL,
                "SKIP_DIRECTORIES should be defined");
    assert_true(SUPPORTED_EXTENSIONS[0] != NULL,
                "SUPPORTED_EXTENSIONS should be defined");

    int skip_count = 0;
    for (int i = 0; SKIP_DIRECTORIES[i] != NULL; i++)
        skip_count++;
    assert_equal_int(skip_count, 8, "Should have 8 skip directories");

    int ext_count = 0;
    for (int i = 0; SUPPORTED_EXTENSIONS[i] != NULL; i++)
        ext_count++;
    assert_equal_int(ext_count, 10, "Should have 10 supported extensions");
}

int main()
{
    printf("Running tests...\n");

    test_config();
    test_has_counted_extension();
    test_is_counted_file();
    test_should_skip_dir();
    test_count_lines();

    printf("\n--- Results ---\n");
    printf("Passed: %d\n", tests_passed);
    printf("Failed: %d\n", tests_failed);

    return tests_failed > 0 ? 1 : 0;
}