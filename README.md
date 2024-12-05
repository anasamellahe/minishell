# Minishell  
**As beautiful as a shell**  

![Shell Screenshot](https://bashlogo.com/img/logo/png/full_colored_dark.png)

## Overview  
Minishell is a simple implementation of a Unix shell, built to provide a deeper understanding of system programming concepts such as process management, file descriptors, and command execution.  

---

## Features  
### Core Functionality  
- **Prompt Display**: Interactive command prompt.  
- **Command Execution**: Executes commands using:  
  - Absolute paths  
  - Relative paths  
  - `PATH` environment variable  
- **Input/Output Redirection**:  
  - `<` Redirect input  
  - `>` Redirect output  
  - `<<` Heredoc mode with delimiter  
  - `>>` Append to file  
- **Pipes**: Command piping with `|`.  
- **Environment Variables**: Expand `$VARIABLE` and `$?`.  
- **Signal Handling**:  
  - `Ctrl-C`: Displays a new prompt.  
  - `Ctrl-D`: Exits the shell.  
  - `Ctrl-\`: Does nothing in interactive mode.  

### Built-in Commands  
- `echo` (with `-n` option)  
- `cd` (relative and absolute paths)  
- `pwd`  
- `export`  
- `unset`  
- `env`  
- `exit`  

### Bonus Features  
- **Logical Operators**: `&&` and `||` with parentheses for prioritization.  
- **Wildcard Support**: `*` in the current directory.  

---

## Installation and Usage  
1. **Clone the repository**:  
   ```bash  
   git clone https://github.com/anasamellahe/minishell.git
   cd minishell
   make
   ./minishell
   ```
---
## Requirements
- Language: C
- Libraries: **readline**
- Compiler: CC

---

## Project Structure

```minishell/  
    - ├── src/             # Source code files  
    - ├── includes/        # Header files  
    - ├── libft/           # Custom library  
    - └──Makefile         # Build instructions
```
---
## Authors
  - Anas amellahe
