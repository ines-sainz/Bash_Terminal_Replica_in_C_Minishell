/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:23:18 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/02 17:53:22 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_number_commands(t_args *args)
{
	t_params	*iter;
	int			n_commands;

	if (args->params->type == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (ERR);
	}
	iter = args->params;
	n_commands = 1;
	while (iter != NULL)
	{
		if (iter->type == PIPE)
		{
			if (iter->next == NULL)
			{
				printf("minishell: syntax error: | at the end of the commands\n");
				return (ERR);
			}
			if (iter->next->type == PIPE)
			{
				printf("minishell: syntax error: two following ||\n");
				return (ERR);
			}
			n_commands++;
		}
		iter = iter->next;
	}
	printf("the number of commands is : %i\n", n_commands);
	return (n_commands);
}

int	redirector(t_args *args, t_mini *mini)
{
	//t_params	*iter;
	int			n_command;

	(void)mini;
	//iter = args->params;
	n_command = get_number_commands(args);
	if (n_command == -1)
		return (1);
/*	while (iter != NULL)
	{
		if (iter->type == PIPE)
		{
			n_command++;
		}
		if (!(iter->type == BUILTING || iter->type == CMD
			|| iter->type == PARAMS))
		{
			//ft_files();
			if (iter)
				iter = iter->next;
		}
		if (iter)
			iter = iter->next;
	}*/
	return (0);
}

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


char	**make_param_matrix(t_params **iter)
{
	char		**param_matrix;
	int			i;
	int			j;

	param_matrix = ft_calloc((get_len_matrix(*iter) + 1), sizeof(char *));
	if (!param_matrix)
		return (0);
	i = 0;
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
	//	if (*iter)
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
}

void	executor(t_args *args)
{
	t_params	*iter;
	char		**param_matrix;
	int			i;
	int			num_command;

	num_command = 1;
	iter = args->params;
	while (iter != NULL)
	{
		if (iter->type != PIPE)
		{
			param_matrix = make_param_matrix(&iter);
			printf("The %i commands are:\n", num_command);
			i = 0;
			while (param_matrix[i])
			{
				printf("command = %s\n", param_matrix[i]);
				i++;
			}
			num_command++;
		}
		//get_the_infile_and_outfile();
		//execute_command(param_matrix, args, mini);
		if (iter)
			iter = iter->next;
	}
}

/*
cat <hola.txt  > /dev/stdout -e
cat <hola.txt -e > /dev/stdout
cat > hola.txt
cat hola.txt
<<<hola.txt cat			-> hola.txt
cat <hola.txt >			->bash: syntax error near unexpected token `newline'
cat <hola.txt >>		->bash: syntax error near unexpected token `newline'
| cat hola.txt			->bash: syntax error near unexpected token `|'
*/