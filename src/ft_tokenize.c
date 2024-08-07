/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/07 10:50:12 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	add_to_args(t_args *args, int *argc, char **arg_ptr)
{
	char	*input_ptr;

	input_ptr = args->input;
	while (*input_ptr)
	{
		if (*input_ptr == '\'' && !(args->in_double_quote))
			args->in_single_quote = !(args->in_single_quote);
		else if (*input_ptr == '\"' && !(args->in_single_quote))
			args->in_double_quote = !(args->in_double_quote);
		else if ((ft_isspace(*input_ptr) || *input_ptr == '|')
			&& !(args->in_single_quote) && !(args->in_double_quote))
		{
			if (*arg_ptr != args->arg)
			{
				**arg_ptr = '\0';
				args->args[(*argc)++] = ft_strdup(args->arg);
				*arg_ptr = args->arg;
			}
			if (*input_ptr == '|')
				args->args[(*argc)++] = ft_strdup("|");
		}
		else
			*(*arg_ptr)++ = *input_ptr;
		input_ptr++;
	}
}

void	ft_tokenize(t_args *args, int *argc)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	add_to_args(args, argc, &args->arg_ptr);
	if (args->arg_ptr != args->arg)
	{
		*(args->arg_ptr) = '\0';
		args->args[(*argc)++] = ft_strdup(args->arg);
	}
	free(args->arg);
}
