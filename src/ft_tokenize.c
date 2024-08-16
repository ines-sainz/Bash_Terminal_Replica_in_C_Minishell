/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/16 18:54:27 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* static void	control_quotes(char	*input_ptr, t_bool	in_single_quote, t_bool	in_double_quote)
{
	if (*input_ptr == '\'' && !in_double_quote)
		in_single_quote = !in_single_quote;
	else if (*input_ptr == '\"' && !in_single_quote)
		in_double_quote = !in_double_quote;
} */

static void	add_to_args(t_args *args, int *argc, char **arg_ptr)
{
	char	*input_ptr;
	char	*expanded_arg;
	char	*next_char;

	input_ptr = args->input;
	while (*input_ptr)
	{
		if (*input_ptr == '\'' && !args->in_double_quote)
			args->in_single_quote = !args->in_single_quote;
		else if (*input_ptr == '\"' && !args->in_single_quote)
			args->in_double_quote = !args->in_double_quote;
		if ((ft_isspace(*input_ptr) || *input_ptr == '|') && !args->in_single_quote && !args->in_double_quote)
		{
			if (*arg_ptr != args->arg)
			{
				**arg_ptr = '\0';
				expanded_arg = expander(args->arg, args->in_single_quote);
				if (expanded_arg)
					args->args[(*argc)++] = expanded_arg;
				*arg_ptr = args->arg;
			}
			if (*input_ptr == '|')
				args->args[(*argc)++] = ft_strdup("|");
		}
		else if (*input_ptr == '$' && !args->in_single_quote)
		{
			next_char = input_ptr + 1;
			if (isdigit(*next_char) || *next_char == '*')
			{
				next_char++;
				input_ptr = next_char;
				continue;
			}
			else
				*(*arg_ptr)++ = *input_ptr;
		}
		else
			*(*arg_ptr)++ = *input_ptr;
		input_ptr++;
	}

	// Aquí verificamos si hay comillas no cerradas
	if (args->in_single_quote)
	{
		*(*arg_ptr)++ = '\''; // Cierra la comilla simple
		args->in_single_quote = false;
	}
	if (args->in_double_quote)
	{
		*(*arg_ptr)++ = '\"'; // Cierra la comilla doble
		args->in_double_quote = false;
	}

	if (*arg_ptr != args->arg)
	{
		**arg_ptr = '\0';
		expanded_arg = expander(args->arg, args->in_single_quote);
		if (expanded_arg)
			args->args[(*argc)++] = expanded_arg;
	}
}

void ft_tokenize(t_args *args, int *argc)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = false;
	args->in_double_quote = false;
	add_to_args(args, argc, &args->arg_ptr);
	free(args->arg);
}
