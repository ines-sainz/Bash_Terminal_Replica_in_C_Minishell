# Replica of a Bash terminal in C (Minishell)

## Authors
Daniel Jiménez Graindorge [https://github.com/BishopVK](https://github.com/BishopVK)

Inés Sainz Ramón [https://github.com/ines-sainz](https://github.com/ines-sainz)


## Description
Minishell is a project that involves creating a simple shell inspired by Bash. Developing this project allows for a deeper understanding of fundamental concepts such as simultaneous processes, file descriptors, and memory management in C. The objetive is to develop a program that, with an interactive prompt and history, can receive, manage and ejecute comands. 

## Features
- Displays an interactive prompt.
- Functional command history.
- Searches and executes commands based on `PATH`, relative paths, or absolute paths.
- Handles quotes and double quotes:
  - Single Quotes `'`: prevents the shell from interpreting metacharacteres.
  - Double Quotes `"`: prevents the shell from interpreting metacharacteres except from `$`
- Implements redirections:
  - `<` redirects input.
  - `>` redirects output.
  - `<<` handles a "heredoc". Reaceives a delimiter that lets you read input from standard input until you add the delimiter.
  - `>>` appends output to a file.
- Implements pipes (`|`) to connect commands. The output of a command is the input of the next one.
- Expands environment variables (`$VARIABLE`).
- Expands `$?` to retrieve the exit status of the last executed command or the last signal.
- Handling of signals as in Bash.:
  -  `ctrl-C`: on a prompt shows a new line with a clear prompt and on a blocking command works as Bash.
  -  `ctrl-D`: on a clear prompt finishes the program, on a prompt does nothing and on a blocking command works as Bash
  -  `ctrl-\`: doesn't do anything except it is on a blocking command where it works as Bash
- Implementation and creation of built-in commands:
  - `echo` (with `-n`): returns the text entered with or without a new line depending on the flag.
  - `cd` (only with absolute or relative paths): allows the user to navigate through directories.
  - `pwd`: returns the path of the directory.
  - `export`: allows the user to create, madify and view environment variables.
  - `unset`: allows the user to eliminate environment variables.
  - `env`: allows the user to view environment variables.
  - `exit`: exits the program in a clear and handled way.

## Evaluation
- Complies with `cc` following the `Norm` coding standard.
- No `segfaults`, `bus errors`, `double frees` or `memory leaks`.
- Proper use of `Makefile` with rules: `all`, `clean`, `fclean`, `re` and flags: `-Wall` `-Werror` and `-Wextra`.

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
