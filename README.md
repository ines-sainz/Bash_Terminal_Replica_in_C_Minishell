# Minishell: A Bash Terminal Replica in C

## Authors
- [Daniel Jiménez Graindorge](https://github.com/BishopVK)
- [Inés Sainz Ramón](https://github.com/ines-sainz)

## Description
Minishell is a project that involves building a shell inspired by Bash, offering a deeper understanding of fundamental operating system concepts. Through its development, I gained hands-on experience with process control, inter-process communication, file descriptors, and dynamic memory management in C. By implementing a command-line interpreter from scratch, this project enhances knowledge of system calls, signal handling, and efficient command execution.

The primary goal is to develop a shell that provides an interactive prompt and command history while correctly handling user input, executing commands, and managing concurrent processes. Additionally, the project enforces good programming practices by ensuring memory safety and following strict coding standards.

## Features
- Displays an interactive prompt.
- Maintains a functional command history.
- Searches and executes commands based on `PATH`, relative paths, or absolute paths.
- Handles single and double quotes:
  - **Single Quotes (`'`)**: Prevents the shell from interpreting metacharacters.
  - **Double Quotes (`"`)**: Prevents the shell from interpreting metacharacters except for `$` (variable expansion).
- Implements redirections:
  - `<` redirects input.
  - `>` redirects output.
  - `<<` (heredoc) reads from standard input until a specified delimiter is encountered.
  - `>>` appends output to a file.
- Implements pipes (`|`) to connect commands, using the output of one command as the input for the next.
- Expands environment variables (`$VARIABLE`).
- Expands `$?` to retrieve the exit status of the last executed command or signal.
- Handles signals as in Bash:
  - **`ctrl-C`**: On an interactive prompt, displays a new line with a clear prompt; on a blocking command, behaves like Bash.
  - **`ctrl-D`**: Exits the shell when the prompt is empty; otherwise, does nothing. On a blocking command behaves like Bash.
  - **`ctrl-\`**: Ignored in the prompt but behaves like Bash in blocking commands.
- Implements built-in commands:
  - `echo` (with `-n`): Prints text with or without a newline.
  - `cd`: Navigates through directories (supports only absolute and relative paths).
  - `pwd`: Displays the current directory path.
  - `export`: Creates, modifies, and lists environment variables.
  - `unset`: Removes environment variables.
  - `env`: Displays environment variables.
  - `exit`: Exits the shell cleanly.

## Installation and Usage

Clone the repository and compile the project:

```sh
$ git clone https://github.com/your_username/minishell.git
$ cd minishell
$ make
```

Run Minishell:

```sh
$ ./minishell
```

To exit, use `exit` or `ctrl-D`.
