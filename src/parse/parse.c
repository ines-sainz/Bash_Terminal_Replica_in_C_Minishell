/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/24 11:33:55 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_tokenize(t_args *args, t_mini *mini)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	args->argc = 0;
	args->in_single_quote = t_false;
	args->in_double_quote = t_false;
	if (add_to_args(args, &args->argc, mini) == ERR)
	{
		free (args->arg);
		args->arg = NULL;
		return (ERR);
	}
	free(args->arg);
	args->arg = NULL;
	return (OK);
}

int	parse(t_args *args, t_mini *mini)
{
	int		i;
	t_bool	heredoc_found;

	heredoc_found = t_false;
	if (ft_tokenize(args, mini) == ERR)
		return (ERR);
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			add_argument_to_list(args, &i, &heredoc_found);
			free(args->args[i]);
			args->args[i] = NULL;
			args->quotes[i] = t_false;
		}
		i++;
	}
	update_last_command_env_var(args);
	new_red_exe(args, mini);
	close_inf_outf(mini);
	return (OK);
}
