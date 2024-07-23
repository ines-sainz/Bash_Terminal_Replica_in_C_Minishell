/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/23 12:30:55 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//static void	add_to_args(char *input, char *args[], int *argc, char **arg_ptr, char *arg)
static void	add_to_args(t_args *args, int *argc, char **arg_ptr)
{
	int	in_single_quote;
	int	in_double_quote;

	in_single_quote = 0;
	in_double_quote = 0;
	while (*(args->input))
	{
		if (*(args->input) == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*(args->input) == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (ft_isspace(*(args->input)) && !in_single_quote && !in_double_quote)
		{
			if (*arg_ptr != args->arg)
			{
				**arg_ptr = '\0';
				args->args[(*argc)++] = ft_strdup(args->arg);
				*arg_ptr = args->arg;
			}
		}
		else
			*(*arg_ptr)++ = *(args->input);
		args->input++;
	}
}

//void	ft_tokenize(char *input, char *args[], int *argc)
void	ft_tokenize(t_args *args, int *argc)
{
	//char	*arg;
	//char	*arg_ptr;

	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	//add_to_args(input, args, argc, &arg_ptr, arg);
	add_to_args(args, argc, &args->arg_ptr);
	if (args->arg_ptr != args->arg)
	{
		*(args->arg_ptr) = '\0';
		args->args[(*argc)++] = ft_strdup(args->arg);
	}
	free(args->arg);
}
