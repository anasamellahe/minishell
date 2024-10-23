# Minishell

As beautiful as a shell.

## Project Overview

Minishell is a project that involves creating a simple shell from scratch. Through this project, you'll gain a deeper understanding of processes and file descriptors while developing your own version of bash.

## Features

- **Prompt Display**: Shows a prompt when waiting for a new command.
- **History**: Keeps a working history of commands.
- **Executable Search**: Finds and runs executables using `PATH` or absolute/relative paths.
- **Quoting**: Handles both single (`'`) and double (`"`) quotes.
- **Redirections**:
  - Input redirection (`<`)
  - Output redirection (`>`)
  - Append mode redirection (`>>`)
  - Heredoc (`<<`)
- **Pipes**: Implements pipes (`|`) to connect commands.
- **Environment Variables**: Supports environment variables and the `$?` status.
- **Signal Handling**: Correctly handles `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` like bash.
- **Builtins**:
  - `echo` (with `-n`)
  - `cd` (with relative/absolute paths)
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`

## Bonus Features

- Logical operators: `&&` and `||` with parentheses for precedence.
- Wildcards: `*` works in the current working directory.

## Usage

To run Minishell, clone the repository and use the following commands:

```bash
make
./minishell
