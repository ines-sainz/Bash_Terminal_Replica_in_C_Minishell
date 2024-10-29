/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_and_history.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 20:17:04 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/29 07:57:09 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	signal_sigint(int sig)
{
	g_signal_received = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_eof(void)
{
	g_signal_received = -1;
}

void	signal_here_doc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = SIGINT;
		rl_on_new_line();
		rl_replace_line("", 1);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
}

int	obtain_last_status(int *status)
{
	if (WTERMSIG(*status) != 0)
	{
		if (WTERMSIG(*status) == 3)
			ft_dprintf(2, "Quit (core dumped)");
		ft_dprintf(2, "\n");
		return (WTERMSIG(*status) + 128);
	}
	return (WEXITSTATUS(*status));
}

void	histcontrol(t_args *args)
{
	if ((args->input[0] != '\0'
			&& ft_strcmp(args->input, args->last_history) != 0)
		|| args->last_history == NULL)
	{
		if (args->last_history != NULL)
			free (args->last_history);
		args->last_history = ft_strdup(args->input);
		add_history(args->input);
	}
}
