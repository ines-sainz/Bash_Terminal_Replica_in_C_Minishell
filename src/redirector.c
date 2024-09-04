/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:08:00 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/04 09:53:41 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirector_syntax_errors(int i)
{
	if (i == 1)
		printf("bash: syntax error near unexpected token `|'\n");
	else if (i == 2)
		printf("minishell: syntax error: | at the end of the commands\n");
	else if (i == 3)
		printf("minishell: syntax error: two following ||\n");
	return (ERR);
}

int	get_number_commands(t_args *args)
{
	t_params	*iter;
	int			n_commands;

	if (args->params->type == PIPE)
		return (redirector_syntax_errors(1));
	iter = args->params;
	n_commands = 1;
	while (iter != NULL)
	{
		if ((iter->type == INFILE || iter->type == HERE_DOC
			|| iter->type == OUTFILE || iter->type == APPEND) && iter->next->type == PIPE)
			return (redirector_syntax_errors(1));
		if (iter->type == PIPE)
		{
			if (iter->next == NULL)
				return (redirector_syntax_errors(2));
			if (iter->next->type == PIPE)
				return (redirector_syntax_errors(3));
			n_commands++;
		}
		iter = iter->next;
	}
	printf("the number of commands is : %i\n", n_commands);
	return (n_commands);
}

/* void	get_redirections(t_args *args, t_mini *mini)
{
	int			n_commands;
	t_params	*iter;

	n_commands = 1;
	iter = args->params;
	while (iter != NULL)
	{
		while (iter->type != PIPE)
		{
			iter = iter->next;
		}
		n_commands++;
		iter = iter->next;
	}
} */

int	redirector(t_args *args, t_mini *mini)
{
//	t_params	*iter;
	int			n_command;

//	(void)mini;
	n_command = get_number_commands(args);
	if (n_command == -1)
		return (1);
	create_pipes(n_command, mini);
	print_pipes(mini);
	//get_redirections(args, mini);
	/*if (n_command == 1)
		execute_one();
	else
		execute_pipex();*/
	return (0);
}
