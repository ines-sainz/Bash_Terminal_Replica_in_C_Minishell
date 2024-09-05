/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:33:21 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/03 11:33:23 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*void	redirect(t_params *iter_params, int n_command, t_args *iter_pipes,
	t_mini *mini)
{
	while (iter_params != NULL && iter_params != PIPE)
	{
		if (iter_params->type == INFILE)
			infile(iter_params->next, iter_pipes, mini);
		if (iter_params->type == HERE_DOC)
			here_doc(iter_params->next, iter_pipes, mini);
		if (iter_params->type == APPEND)
			append(iter_params->next, iter_pipes, mini);
		if (iter_params->type == OUTFILE)
			outfile(iter_params->next, iter_pipes, mini);
	}
}*/

void	get_redirections(t_args *args, t_mini *mini)
{
	t_params	*iter_params;
	t_pipes		*iter_pipes;
	int			n_commands;

	iter_params = args->params;
	iter_pipes = mini->first_pipe;
	n_commands = 1;
	while (iter_params != NULL)
	{
		//redirect(iter_params, );
		while (iter_params != PIPE || iter_params->next != NULL)
			iter_params = iter_params->next;
		iter_pipes = iter_pipes->next;
		iter_params = iter_params->next;
	}
}
