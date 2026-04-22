# loc-counter

A lightweight command-line tool written in C to count lines of code in a repository, recursively walking directories and filtering by file type.

## Features

- Recursively counts lines across entire project directories
- Filters by file extension (`.py`, `.ts`, `.tsx`, `.js`, `.jsx`, `.vue`, `.yml`, `.yaml`, `.toml` , `.c`)
- Counts `Dockerfile` and `docker-compose` files
- Automatically skips common directories: `.git`, `node_modules`, `__pycache__`, `migrations`
- Skips hidden files (starting with `.`)
- Fast, single-pass implementation

## Supported Languages/Files

- Python (`.py`)
- TypeScript (`.ts`, `.tsx`)
- JavaScript (`.js`, `.jsx`)
- Vue (`.vue`)
- YAML (`.yml`, `.yaml`)
- TOML (`.toml`)
- Docker (`Dockerfile`, `docker-compose.*`)

## Building

Requirements: `gcc`, standard C library

```bash
gcc main.c -o loc
```

## Usage

Count lines in current directory:
```bash
./loc .
```

Count lines in a specific directory:
```bash
./loc /path/to/project
```

## Installation (Optional)

Copy the binary to your PATH for system-wide access:

```bash
gcc main.c -o loc
sudo cp loc /usr/local/bin/
```

Then use from anywhere:
```bash
loc ~/projects/myapp
```

## How It Works

1. Opens the directory specified on the command line
2. Recursively walks subdirectories (skipping blacklisted folders)
3. For each file, checks if the extension matches supported types
4. Counts newline characters to determine line count
5. Prints total lines of code

## Example Output

```
$ loc .
Total lines: 2847
```

## Notes

- Empty lines are counted (they're still lines)
- Files must end with a newline character to be fully counted
- Hidden files and directories (starting with `.`) are automatically skipped
- If a file or directory can't be read, an error is printed but execution continues