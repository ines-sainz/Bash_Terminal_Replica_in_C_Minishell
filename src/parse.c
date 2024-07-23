/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/23 12:19:03 by danjimen &       ###   ########.fr       */
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

int	parse(t_args *args, t_mini *mini)
{
	//char	*args[MAX_ARGS];
	//int		argc;
	int		i;

	(void)mini;
	printf("You entered: %s\n", args->input);
	//ft_tokenize(input, args, &argc);
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