/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/22 14:01:42 by danjimen &       ###   ########.fr       */
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

int	parse(char *input, char **env)
{
	char	*args[MAX_ARGS];
	t_mini	mini;
	int		argc;
	int		i;

	printf("You entered: %s\n", input);
	ft_bzero(&mini, sizeof(t_mini));
	ft_set_env(env, &mini);	///TENEMOS QUE SETEAR LA VARIABLE EN EL MAIN
	ft_tokenize(input, args, &argc);
	printf("argc ==> %i\n", argc);

	printf("Parsed arguments:\n");
	i = 0;
	while (i < argc)
	{
		printf("arg[%d]: %s\n", i, args[i]);
		free(args[i]);
		i++;
	}
	return (0);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/