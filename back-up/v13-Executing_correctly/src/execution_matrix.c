/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_matrix.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:43:20 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/17 09:43:22 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_len_matrix(t_params *iter)
{
	t_params	*temp;
	int			n_commands; //es el número de el comando más todo lo que hay detrás

	temp = iter;
	n_commands = 0;
	while (temp != NULL)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == BUILTING || temp->type == CMD
			|| temp->type == PARAMS)
		{
			n_commands++;
		}
		else if (temp)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	return (n_commands);
}


char	**make_param_matrix(t_params **iter, int i)
{
	char		**param_matrix;
	int			j;

	param_matrix = ft_calloc((get_len_matrix(*iter) + 1), sizeof(char *));
	if (!param_matrix)
		return (0);
	while (*iter != NULL)
	{
		if ((*iter)->type == PIPE)
			break ;
		if ((*iter)->type == BUILTING || (*iter)->type == CMD
			|| (*iter)->type == PARAMS)
		{
			param_matrix[i] = ft_calloc((ft_strlen((*iter)->content) + 1), 1);
			if (!param_matrix[i])
				return (0);
			j = 0;
			while ((*iter)->content[j])
			{
				param_matrix[i][j] = (*iter)->content[j];
				j++;
			}
			i++;
		}
		else
			(*iter) = (*iter)->next;
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
}

void	fill_exe(t_params *iter_params, t_execution *iter_exe)
{
	int	i;

	while (iter_params != NULL)
	{
		if (iter_params->type != PIPE)
		{
			i = 0;
			iter_exe->command = make_param_matrix(&iter_params, i);
			iter_exe = iter_exe->next;
		}
		if (iter_params)
			iter_params = iter_params->next;
	}
}