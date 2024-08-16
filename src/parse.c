/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/08/16 20:18:59 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define MAX_ARGS 100

/* int	analize_parts(char *input)
{
	analize_simple_quotes();
	analize_double_quotes();
	analize_infile();
	analize_outfile();
	analize_built_in();
} */

static void	ft_tokenize(t_args *args, int *argc)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = false;
	args->in_double_quote = false;
	add_to_args(args, argc);
	free(args->arg);
}

int	parse(t_args *args, t_mini *mini)
{
	int	i;

	(void)mini;
	printf("You entered: %s\n", args->input);
	ft_tokenize(args, &args->argc);
	printf("argc ==> %i\n", args->argc);
	printf("Parsed arguments:\n");
	i = 0;
	while (i < args->argc)
	{
		printf("arg[%d]: %s\n", i, args->args[i]);
		free(args->args[i]);
		i++;
	}
	return (0);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/