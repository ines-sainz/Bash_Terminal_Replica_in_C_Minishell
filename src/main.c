/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 15:25:44 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/12 13:54:36 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Manejador de la señal SIGINT (Ctrl-C)
void	signal_sigint(int sig)
{
	(void)sig;
	//printf("\nCaught signal %d (Ctrl-C). Exiting...\n", sig);
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Manejador de la señal SIGQUIT (Ctrl-\)
void	signal_sigquit(int sig)
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
	signal(SIGINT, signal_sigint);
	signal(SIGQUIT, signal_sigquit);

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
	clear_history();
	return (0);
}
