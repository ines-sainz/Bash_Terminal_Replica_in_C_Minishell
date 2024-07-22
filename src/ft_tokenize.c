/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/22 14:54:12 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

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

void	ft_tokenize(char *input, char *args[], int *argc)
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
