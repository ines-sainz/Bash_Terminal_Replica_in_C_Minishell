/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:08:00 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/02 18:08:03 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	redirector_syntax_errors(int i, char *red_content)
{
	if (i == 1)
		printf("bash: syntax error near unexpected token `|'\n");
	else if (i == 2)
		printf("minishell: syntax error: | at the end of the commands\n");
	else if (i == 3)
		printf("bash: syntax error near unexpected token `newline'\n");
	else if (i == 4)
		printf("bash: syntax error near unexpected token `%s'\n", red_content);
	return (ERR);
}

int	get_number_commands(t_args *args)
{
	t_params	*iter;
	int			n_commands;

	if (args->params->type == PIPE)
		return (redirector_syntax_errors(1, args->params->content));
	iter = args->params;
	n_commands = 1;
	while (iter != NULL)
	{
		if (iter->type == INFILE || iter->type == HERE_DOC
			|| iter->type == OUTFILE || iter->type == APPEND)
		{
			if (iter->next == NULL)
				return (redirector_syntax_errors(3, iter->content));
			if (iter->next->type == PIPE)
				return (redirector_syntax_errors(1, iter->next->content));
			if (iter->next->type == INFILE || iter->next->type == HERE_DOC
				|| iter->next->type == OUTFILE || iter->next->type == APPEND)
				return (redirector_syntax_errors(4, iter->next->content));
		}
		if (iter->type == PIPE)
		{
			if (iter->next == NULL)
				return (redirector_syntax_errors(2, iter->content));
			n_commands++;
		}
		iter = iter->next;
	}
	printf("the number of commands is : %i\n", n_commands);
	return (n_commands);
}

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
