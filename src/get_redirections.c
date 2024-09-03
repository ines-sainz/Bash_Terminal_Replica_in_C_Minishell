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

/*void	get_redirections(t_args *args, t_mini *mini)
{
	t_params	*iter_params;
	t_pipes		*iter_pipes;
	int			n_commands;

	iter_params = args->params;
	iter_pipes = mini->first_pipe;
	n_commands = 1;
	if (iter_params == INFILE || iter_params == OUTFILE
		|| iter_params == APPEND)
	{
		redirections(iter_params, mini);
		iter_params = iter_params->next;
	}
	while (iter_params != NULL)
	{
		while (iter_params->type != PIPE)
		{
			if (iter_params == INFILE || iter_params == OUTFILE ||
				iter_params == APPEND iter_params->next = )
			iter_params = iter_params->next;
		}
		n_commands++;
		iter_params = iter_params->next;
	}
}*/
