/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 18:24:51 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/02 18:24:53 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	print_pipes(t_mini *mini)
{
	t_pipes	*iter;

	iter = mini->first_pipe;
	while (iter != NULL)
	{
		printf("num_pipes_command: %i, inf: %i, outf: %i\n", iter->num_command, iter->inf_pipe, iter->outf_pipe);
		iter = iter->next;
	}
}

void	free_pipes(t_mini *mini)
{
	t_pipes	*next;
	t_pipes	*iter;

	iter = mini->first_pipe;
	if (iter)
	{
		while(iter != NULL)
		{
			next = iter->next;
			free(iter);
			iter = next;
		}
	}
	mini->first_pipe = NULL;
}

t_pipes	*pipe_new(int i)
{
	t_pipes	*node;

	node = (t_pipes *)malloc(sizeof(t_pipes));
	if (!node)
		return (NULL);
	node->num_command = i;
	node->inf_pipe = 0;
	node->outf_pipe = 1;
	node->next = NULL;
	return (node);
}

void	add_back_pipe(t_pipes *node, t_mini *mini)
{
	t_pipes	*iter;

	if (!mini->first_pipe)
	{
		mini->first_pipe = node;
		return ;
	}
	iter = mini->first_pipe;
	while(iter->next != NULL)
		iter = iter->next;
	iter->next = node;
	return ;
}

void	create_pipes(int n_command, t_mini *mini)
{
	t_pipes	*node;
	int		i;

	mini->first_pipe = NULL;
	i = 1;
	while (i <= n_command)
	{
		node = pipe_new(i);
		if (!node)
			free_pipes(mini);
		add_back_pipe(node, mini);
		i++;
	}
}
