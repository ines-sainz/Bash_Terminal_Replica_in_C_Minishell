/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/08/23 21:25:26 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define MAX_ARGS 100

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
	int	i;

	//(void)mini;
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
			free(args->args[i]);
		}
		i++;
	}
	return (0);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/