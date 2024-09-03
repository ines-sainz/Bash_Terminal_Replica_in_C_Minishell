/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/03 13:49:12 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	next_is_a_number(char **input_ptr, t_args *args)
{
	char	*next_char;

	next_char = *input_ptr + 1;
	if (ft_isdigit(*next_char) || *next_char == '*')
		*input_ptr = next_char;
	else
		*args->arg_ptr++ = **input_ptr;
}

static int	out_of_quotes(char **input_ptr, t_args *args,
	int *argc, t_mini *mini)
{
	char	*expanded_arg;

	if (args->arg_ptr != args->arg)
	{
		*args->arg_ptr = '\0';
		expanded_arg = expander(args, mini);
		if (expanded_arg)
		{
			args->args[(*argc)++] = expanded_arg;
			//free(expanded_arg);
			//expanded_arg = NULL;
		}
		args->arg_ptr = args->arg;
	}
	if (**input_ptr == '|' && *(*input_ptr + 1) == '|')
	{
		printf("minishell: syntax error: || it's not allowed\n");
		return (ERR);
	}
	else if (**input_ptr == '|')
		args->args[(*argc)++] = ft_strdup("|");
	else if (**input_ptr == '<' && *(*input_ptr + 1) == '<' && *(*input_ptr + 2) == '<')
	{
		printf("minishell: syntax error near unexpected token `<'\n");
		return (ERR);
	}
	else if ((**input_ptr == '<' && *(*input_ptr + 1) == '>')
			|| (**input_ptr == '>' && *(*input_ptr + 1) == '<'))
	{
		printf("minishell: syntax error: forbidden token '<>' or '><'\n");
		return (ERR);
	}
	else if (**input_ptr == '<' && *(*input_ptr + 1) == '<')
	{
		args->args[(*argc)++] = ft_strdup("<<");
		(*input_ptr)++;
	}
	else if (**input_ptr == '<')
		args->args[(*argc)++] = ft_strdup("<");
	else if (**input_ptr == '>' && *(*input_ptr + 1) == '>' && *(*input_ptr + 2) == '>')
	{
		printf("minishell: syntax error near unexpected token `>'\n");
		return (ERR);
	}
	else if (**input_ptr == '>' && *(*input_ptr + 1) == '>')
	{
		args->args[(*argc)++] = ft_strdup(">>");
		(*input_ptr)++;
	}
	else if (**input_ptr == '>')
		args->args[(*argc)++] = ft_strdup(">");
	return (OK);
}

static void	control_quotes(char *input_ptr, t_args *args, int *flag)
{
	if (*input_ptr == '\'' && !args->in_double_quote)
	{
		//printf("Entre comilla simple\n");
		args->in_single_quote = !args->in_single_quote;
		(*flag)++;
		if (*flag % 2 != 0)
			args->quotes[args->argc] = !args->quotes[args->argc];
	}
	else if (*input_ptr == '\"' && !args->in_single_quote)
	{
		//printf("Entre comilla doble\n");
		args->in_double_quote = !args->in_double_quote;
		(*flag)++;
		if (*flag % 2 != 0)
			args->quotes[args->argc] = !args->quotes[args->argc];
	}
	// printf("flag = %i\n", *flag);
	// printf("args->quotes[%i] ==> %i\n", args->argc, args->quotes[args->argc]);
}

/* static void	verify_closed_quotes(t_args *args)
{
	if (args->in_single_quote)
	{
		*args->arg_ptr++ = '\'';
		args->in_single_quote = t_false;
	}
	if (args->in_double_quote)
	{
		*args->arg_ptr++ = '\"';
		args->in_double_quote = t_false;
	}
} */

static int	verify_closed_quotes(t_args *args)
{
	if (args->in_single_quote || args->in_double_quote)
	{
		printf("minishell: syntax error: missing closed quotes\n");
		//free_at_exit(args);
		return (ERR);
	}
	return (OK);
}

int	add_to_args(t_args *args, int *argc, t_mini *mini)
{
	char	*input_ptr;
	char	*expander_arg;
	int		flag;

	flag = 0;
	args->quotes[args->argc] = t_false;
	input_ptr = args->input;
	while (*input_ptr)
	{
		control_quotes(input_ptr, args, &flag);
		if ((ft_isspace(*input_ptr) || *input_ptr == '|' || *input_ptr == '<'
				|| *input_ptr == '>')
			&& !args->in_single_quote && !args->in_double_quote)
			{
				if (out_of_quotes(&input_ptr, args, argc, mini) == ERR)
				return (ERR);
			}
		else if (*input_ptr == '$' && !args->in_single_quote)
			next_is_a_number(&input_ptr, args);
		else
			*args->arg_ptr++ = *input_ptr;
		input_ptr++;
	}
	if (verify_closed_quotes(args) == ERR)
		return (ERR);
	if (args->arg_ptr != args->arg)
	{
		*args->arg_ptr = '\0';
		expander_arg = expander(args, mini);
		if (expander_arg)
		{
			args->args[(*argc)++] = expander_arg;
			//free (expander_arg);
			//expander_arg = NULL;
		}
	}
	return (OK);
}
