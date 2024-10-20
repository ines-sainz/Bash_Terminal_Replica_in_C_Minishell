/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_matrix_copy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 09:43:20 by isainz-r          #+#    #+#             */
/*   Updated: 2024/10/17 09:53:25 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_count_elements(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

int	get_len_matrix(t_params *iter)
{
	t_params	*temp;
	int			n_commands;

	temp = iter;
	n_commands = 0;
	while (temp != NULL)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == BUILTING || temp->type == CMD
			|| temp->type == PARAMS)
		{
			if (temp->type == t_false)
				n_commands += ft_count_elements(temp->content, ' ');
			else
				n_commands++;
		}
		else if (temp)
			temp = temp->next;
		printf("DB: Nº parametros => %i\n", n_commands);
		printf("DB: Parametro => %s\n", temp->content);
		if (temp)
			temp = temp->next;
	}
	return (n_commands);
}

/* int	get_len_matrix(t_params *iter)
{
	t_params	*temp;
	int			n_commands;

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
} */

char	**make_param_matrix(t_params **iter, int i)
{
	char	**param_matrix;
	char	**arg_splited;
	int		j;

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
			if ((*iter)->type == t_false)
			{
				arg_splited = ft_split((*iter)->content, ' ');
				j = 0;
				while (arg_splited[j] != NULL)
				{
					param_matrix[i] = ft_strdup(arg_splited[j]);
					printf ("DB: param_matrix[%i] => %s\n", i, param_matrix[i]);
					free(arg_splited[j]);
					i++;
					j++;
				}
				free(arg_splited);
			}
			else
			{
				param_matrix[i] = ft_strdup((*iter)->content);
				if (!param_matrix[i])
					return (0);
				i++;
			}
		}
		else
			(*iter) = (*iter)->next;
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
}

/* char	**make_param_matrix(t_params **iter, int i)
{
	char	**param_matrix;

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
			param_matrix[i] = ft_strdup((*iter)->content);
			if (!param_matrix[i])
				return (0);
			i++;
		}
		else
			(*iter) = (*iter)->next;
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
} */

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
