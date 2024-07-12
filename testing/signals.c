/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 10:38:22 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/12 10:07:46 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

// Manejador de la señal SIGINT (Ctrl-C)
void	handle_sigint(int sig)
{
	printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	clear_history();
	exit (0);
}

// Manejador de la señal SIGQUIT (Ctrl-\)
void	handle_sigquit(int sig)
{
	printf("\nCaught signal %d (Ctrl-\\). Dumping core and exiting...\n", sig);
	clear_history();
	signal(sig, SIG_DFL);  // Restaurar el comportamiento por defecto
	kill(getpid(), sig);   // Enviar la señal nuevamente
}

// cc signals.c -lreadline
int	main(void)
{
	char	*input;

	// Configurar los manejadores de señal
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);

	// Bucle principal del shell
	while (1)
	{
		input = readline("shell> ");
		if (!input)
		{
			// Detectar Ctrl-D (EOF)
			printf("\nCaught EOF (Ctrl-D). Exiting...\n");
			break ;
		}

		if (input[0] != '\0')
			add_history(input);

		// Procesar la entrada del usuario
		printf("You entered: %s\n", input);

		free(input);  // Liberar la memoria asignada por readline
	}

	return (0);
}

