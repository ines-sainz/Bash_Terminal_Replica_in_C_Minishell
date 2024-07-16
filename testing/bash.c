/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 14:06:05 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/16 14:06:24 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

// Función para manejar señales (Ctrl+C)
void handle_signal(int sig)
{
	if (sig == SIGINT) {
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

// Función para ejecutar comandos
void execute_command(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Proceso hijo
		if (execvp(args[0], args) == -1) {
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error en fork
		perror("lsh");
	} else {
		// Proceso padre
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}
}

// Función para leer la línea de comandos
char *read_line(void)
{
	char *line = readline("lsh> ");
	if (line && *line) {
		add_history(line);
	}
	return line;
}

// Función para parsear la línea de comandos en argumentos
char **split_line(char *line)
{
	int bufsize = 64, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, " \t\r\n\a");
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += 64;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, " \t\r\n\a");
	}
	tokens[position] = NULL;
	return tokens;
}

int main(int argc, char **argv)
{
	char *line;
	char **args;
	int status;

	// Manejar señales
	signal(SIGINT, handle_signal);

	// Loop principal del shell
	do {
		line = read_line();
		if (!line) {
			break;
		}
		args = split_line(line);
		if (args[0] != NULL) {
			execute_command(args);
		}
		free(line);
		free(args);
	} while (1);

	return 0;
}