/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_out_of_quotes.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 11:51:37 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/20 21:59:09 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_pipes(char **input_ptr, t_args *args,
	int *argc, t_mini *mini)
{
	if (**input_ptr == '|' && *(*input_ptr + 1) == '|')
	{
		ft_dprintf(2, "minishell: syntax error: || it's not allowed\n");
		ft_export_env("?=2", mini);
		return (ERR);
	}
	else if (**input_ptr == '|' && *argc == 0)
	{
		ft_dprintf(2, "bash: syntax error near unexpected token `|'\n");
		ft_export_env("?=2", mini);
		return (ERR);
	}
	else if (**input_ptr == '|')
		args->args[(*argc)++] = ft_strdup("|");
	return (OK);
}

static int	handle_infiles(char **input_ptr, t_args *args,
	int *argc, t_mini *mini)
{
	if (**input_ptr == '<' && *(*input_ptr + 1) == '<'
		&& (*(*input_ptr + 2) == '<' || *(*input_ptr + 2) == '>'
			|| *(*input_ptr + 2) == '|'))
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `<'\n");
		ft_export_env("?=2", mini);
		return (ERR);
	}
	else if ((**input_ptr == '<' && *(*input_ptr + 1) == '>')
		|| (**input_ptr == '>' && *(*input_ptr + 1) == '<'))
	{
		ft_dprintf(2,
			"minishell: syntax error: forbidden token '<>' or '><'\n");
		ft_export_env("?=2", mini);
		return (ERR);
	}
	else if (**input_ptr == '<' && *(*input_ptr + 1) == '<')
	{
		args->args[(*argc)++] = ft_strdup("<<");
		args->in_heredoc = t_true; // Marcar que estamos en un heredoc
		(*input_ptr)++;
	}
	else if (**input_ptr == '<')
		args->args[(*argc)++] = ft_strdup("<");
	return (OK); // Comprobar a la hora de marcar el delimitador si comienza por <, >, ;, & o | y no está entre comillas
}

static int	handle_outfiles(char **input_ptr, t_args *args,
	int *argc, t_mini *mini)
{
	if (**input_ptr == '>' && *(*input_ptr + 1) == '>' && *(*input_ptr + 2) == '>')
	{
		ft_dprintf(2, "minishell: syntax error near unexpected token `>'\n");
		ft_export_env("?=2", mini);
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

int	out_of_quotes(char **input_ptr, t_args *args,
	int *argc, t_mini *mini)
{
	char	*expanded_arg;

	if (args->arg_ptr != args->arg)
	{
		*args->arg_ptr = '\0';
		expanded_arg = expander(args, mini);
		if (expanded_arg)
			args->args[(*argc)++] = expanded_arg;
		args->arg_ptr = args->arg;
	}
	if (handle_pipes(input_ptr, args, argc, mini) == ERR)
		return (ERR);
	if (handle_infiles(input_ptr, args, argc, mini) == ERR)
		return (ERR);
	if (handle_outfiles(input_ptr, args, argc, mini) == ERR)
		return (ERR);
	return (OK);
}
