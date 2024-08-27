/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/08/27 20:34:09 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define MAX_ARGS 100

void	del_params(t_args *args)
{
	t_params	*current;
	t_params	*next;

	current = args->params;
	if (args->params != NULL)
	{
		while (current != NULL)
		{
			next = current->next;
			if (current->content != NULL)
				free(current->content);
			free(current);
			current = next;
		}
		args->params = NULL;
	}
}

static void	ft_tokenize(t_args *args, int *argc, t_mini *mini)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = t_false;
	args->in_double_quote = t_false;
	add_to_args(args, argc, mini);
	free(args->arg);
}

int	parse(t_args *args, t_mini *mini)
{
	int			i;
	t_params	*temp;

	printf("You entered: %s\n", args->input);
	ft_tokenize(args, &args->argc, mini);
	printf("argc ==> %i\n", args->argc);
	if (ft_built_ins(args, mini) == 1)
		return (0);
	printf("Parsed arguments:\n");
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			printf("arg[%d]: %s\n", i, args->args[i]);
			add_argument_to_list(args, &i);
			free(args->args[i]);
		}
		i++;
	}
	temp = args->params;
	while (temp != NULL)
	{
		printf("arg[%d]: %s, type: %d\n", temp->argc, temp->content, temp->type);
		temp = temp->next;
	}
	return (0);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/