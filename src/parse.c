/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/22 13:13:51 by danjimen &       ###   ########.fr       */
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

static void	add_to_args(char *input, char *args[], int *argc, char **arg_ptr, char *arg)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (ft_isspace(*input) && !in_single_quote && !in_double_quote)
		{
			if (*arg_ptr != arg)
			{
				**arg_ptr = '\0';
				args[(*argc)++] = ft_strdup(arg);
				*arg_ptr = arg;
			}
		}
		else
			*(*arg_ptr)++ = *input;
		input++;
	}
}

static void	ft_tokenize(char *input, char *args[], int *argc)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*arg;
	char	*arg_ptr;

	in_single_quote = 0;
	in_double_quote = 0;
	arg = malloc(ft_strlen(input) + 1);
	arg_ptr = arg;
	*argc = 0;
	add_to_args(input, args, argc, &arg_ptr, arg);
	if (arg_ptr != arg)
	{
		*arg_ptr = '\0';
		args[(*argc)++] = ft_strdup(arg);
	}
	free(arg);
}

int	parse(char *input)
{
	char	*args[MAX_ARGS];
	t_mini	mini;
	int		argc;
	int		i;

	printf("You entered: %s\n", input);
	ft_bzero(&mini, sizeof(t_mini));
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