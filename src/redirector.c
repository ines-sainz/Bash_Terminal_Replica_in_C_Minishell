/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 15:01:59 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/16 15:02:03 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	syntax_errors(int i, char *redir_content, t_mini *mini)
{
	if (i == 1)
		printf("minishell: syntax error near unexpected token `|'\n");
	else if (i == 2)
		printf("minishell: syntax error near unexpected token `newline'\n");
	else if (i == 3)
		printf("minishell: syntax error near unexpected token `%s'\n",
			redir_content);
	else if (i == 4)
			printf("minishell: syntax error: | at the end of the commands\n");
	ft_export_env("?=2", mini);
	return (ERR);
}

int	errors_and_n_commands(t_params *iter, t_mini *mini)
{
	if (iter->type == '|')
		return(syntax_errors(1, iter->content, mini));
	while (iter != NULL)
	{
		if (iter->type == INFILE || iter->type == OUTFILE
			|| iter->type == HERE_DOC || iter->type == APPEND)
		{
			if (iter->next == NULL)
				return (syntax_errors(2, iter->content, mini));
			if (iter->next->type == PIPE)
				return (syntax_errors(1, iter->content, mini));
			if (iter->next->type == INFILE || iter->next->type == HERE_DOC
				|| iter->next->type == OUTFILE || iter->next->type == APPEND)
				return (syntax_errors(3, iter->next->content, mini));
		}
		if (iter->type == PIPE)
		{
			if (iter->next == NULL)
				return (syntax_errors(4, iter->content, mini));
			mini->n_commands++;
		}
		iter = iter->next;
	}
	printf("DB: the number of commands is: %i\n", mini->n_commands);
	return (0);
}

void	fill_exe_redirections(t_params *iter_params, t_execution *iter_exe, t_args *args, t_mini *mini)
{
	int			*here_doc_fds;

	here_doc_fds = get_here_doc(iter_params, args);
	iter_params = args->params;
	mini->n_here_docs = 0;
	while (iter_params)
	{
		if (iter_params->type == INFILE)
			fill_infile(iter_params->next, iter_exe, mini);
		else if (iter_params->type == HERE_DOC)
		{
			fill_here_doc(here_doc_fds[mini->n_here_docs], iter_exe, mini);
			mini->n_here_docs++;
		}
		else if (iter_params->type == APPEND)
			fill_append(iter_params->next, iter_exe, mini);
		else if (iter_params->type == OUTFILE)
			fill_outfile(iter_params->next, iter_exe, mini);
		else if (iter_params->type == PIPE)
		{
			fill_pipe(iter_exe);
			iter_exe = iter_exe->next;
		}
		iter_params = iter_params->next;
	}
}
