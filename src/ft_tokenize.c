/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/20 15:08:15 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	dollar_out_of_single_quotes(char **input_ptr, t_args *args)
{
	char	*next_char;

	next_char = *input_ptr + 1;
	if (ft_isdigit(*next_char) || *next_char == '*')
		*input_ptr = next_char;
	else
		*args->arg_ptr++ = **input_ptr;
}

static void	out_of_quotes(char *input_ptr, t_args *args, int **argc)
{
	char	*expanded_arg;

	if (args->arg_ptr != args->arg)
	{
		*args->arg_ptr = '\0';
		expanded_arg = expander(args->arg, args->in_single_quote);
		if (expanded_arg)
			args->args[(**argc)++] = expanded_arg;
		args->arg_ptr = args->arg;
	}
	if (*input_ptr == '|')
		args->args[(**argc)++] = ft_strdup("|");
}

static void	control_quotes(char *input_ptr, t_args *args)
{
	if (*input_ptr == '\'' && !args->in_double_quote)
		args->in_single_quote = !args->in_single_quote;
	else if (*input_ptr == '\"' && !args->in_single_quote)
		args->in_double_quote = !args->in_double_quote;
}

static void	verify_closed_quotes(t_args *args)
{
	if (args->in_single_quote)
	{
		*args->arg_ptr++ = '\'';
		args->in_single_quote = false;
	}
	if (args->in_double_quote)
	{
		*args->arg_ptr++ = '\"';
		args->in_double_quote = false;
	}
}

void	add_to_args(t_args *args, int *argc)
{
	char	*input_ptr;
	char	*expanded_arg;

	input_ptr = args->input;
	while (*input_ptr)
	{
		control_quotes(input_ptr, args);
		if ((ft_isspace(*input_ptr) || *input_ptr == '|')
			&& !args->in_single_quote && !args->in_double_quote)
			out_of_quotes(input_ptr, args, &argc);
		//else if (*input_ptr == '$' && *input_ptr - 1 == '$' && !args->in_single_quote)
		else if (*input_ptr == '$' && !args->in_single_quote)
			dollar_out_of_single_quotes(&input_ptr, args);
		else
			*args->arg_ptr++ = *input_ptr;
		input_ptr++;
	}
	verify_closed_quotes(args);
	if (args->arg_ptr != args->arg)
	{
		*args->arg_ptr = '\0';
		expanded_arg = expander(args->arg, args->in_single_quote);
		if (expanded_arg)
			args->args[(*argc)++] = expanded_arg;
	}
}
