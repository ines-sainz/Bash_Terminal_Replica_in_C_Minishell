/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/01 23:06:34 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define MAX_ARGS 100

static int	ft_tokenize(t_args *args, int *argc, t_mini *mini)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = t_false;
	args->in_double_quote = t_false;
	if (add_to_args(args, argc, mini) == ERR)
	{
		free (args->arg);
		args->arg = NULL;
		return (ERR);
	}
	free(args->arg);
	args->arg = NULL;
	return (OK);
}

int	parse(t_args *args, t_mini *mini)
{
	int			i;
	t_params	*temp;

	printf("You entered: %s\n", args->input);
	if (ft_tokenize(args, &args->argc, mini) == ERR)
		return (ERR);
	printf("argc ==> %i\n", args->argc);
	if (ft_built_ins(args, mini) == 1)
		return (OK);
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			//printf("arg[%d]: %s\n", i, args->args[i]);
			add_argument_to_list(args, &i);
			free(args->args[i]);
			args->args[i] = NULL;
		}
		i++;
	}
	if (redirector(args, mini) == 1)
		return (ERR);
	executor(args);
	printf("Parsed arguments:\n");
	temp = args->params;
	while (temp != NULL)
	{
		printf("arg[%d]: %s, type: %d\n", temp->argc, temp->content, temp->type);
		temp = temp->next;
	}
	return (OK);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/