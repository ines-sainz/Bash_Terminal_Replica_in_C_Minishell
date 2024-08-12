/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/12 22:18:11 by danjimen         ###   ########.fr       */
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
	t_bool	in_single_quote = false;
	t_bool	in_double_quote = false;

	input_ptr = args->input;
	while (*input_ptr)
	{
		//control_quotes(input_ptr, in_single_quote, in_double_quote);
		if (*input_ptr == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input_ptr == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if ((ft_isspace(*input_ptr) || *input_ptr == '|') && !in_single_quote && !in_double_quote)
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
		else
			*(*arg_ptr)++ = *input_ptr;
		input_ptr++;
	}
	if (*arg_ptr != args->arg)
	{
		**arg_ptr = '\0';
		expanded_arg = expander(args->arg, args->in_single_quote);
		if (expanded_arg)
			args->args[(*argc)++] = expanded_arg;
	}
}

//Funciona bien pero con error con variables cuyo contenido es muy largo
/* static void add_to_args(t_args *args, int *argc, char **arg_ptr)
{
    char *input_ptr;
    char *expanded_arg;
    bool in_single_quote = false;
    bool in_double_quote = false;

    input_ptr = args->input;
    while (*input_ptr)
    {
        if (*input_ptr == '\'' && !in_double_quote)
            in_single_quote = !in_single_quote;
        else if (*input_ptr == '\"' && !in_single_quote)
            in_double_quote = !in_double_quote;
        
        if ((ft_isspace(*input_ptr) || *input_ptr == '|') && !in_single_quote && !in_double_quote)
        {
            if (*arg_ptr != args->arg)
            {
                **arg_ptr = '\0';
                expanded_arg = expander(args->arg, args->in_single_quote);
                args->args[(*argc)++] = expanded_arg;
                *arg_ptr = args->arg;
            }
            if (*input_ptr == '|')
                args->args[(*argc)++] = ft_strdup("|");
        }
        else
        {
            *(*arg_ptr)++ = *input_ptr;
        }
        input_ptr++;
    }

    if (*arg_ptr != args->arg)
    {
        **arg_ptr = '\0';
        expanded_arg = expander(args->arg, args->in_single_quote);
        args->args[(*argc)++] = expanded_arg;
    }
} */

/* void ft_tokenize(t_args *args, int *argc)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = false;
	args->in_double_quote = false;
	add_to_args(args, argc, &args->arg_ptr);

	// Expansión de variables después de separar en argumentos
	for (int i = 0; i < *argc; i++)
	{
		char *arg = args->args[i];
		bool in_single_quote = false;
		bool in_double_quote = false;
		char *expanded_arg = malloc(ft_strlen(arg) * 2); // Asumimos el peor caso, necesitar el doble de espacio
		char *exp_ptr = expanded_arg;

		for (char *c = arg; *c; c++)
		{
			if (*c == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (*c == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
			else if (*c == '$' && !in_single_quote)
			{
				char var_name[100]; // Tamaño máximo asumido de nombre de variable
				int var_len = 0;
				c++;
				while (*c && (isalnum(*c) || *c == '_'))
				{
					var_name[var_len++] = *c++;
				}
				var_name[var_len] = '\0';
				c--;

				char *env_value = getenv(var_name);
				if (env_value)
				{
					strcpy(exp_ptr, env_value);
					exp_ptr += strlen(env_value);
				}
			}
			else
				*exp_ptr++ = *c;
		}
		*exp_ptr = '\0';

		if (arg != expanded_arg)
		{
			free(args->args[i]);
			args->args[i] = ft_strdup(expanded_arg);
		}
		free(expanded_arg);
	}
	
	free(args->arg);
} */

//Funciona bien pero con error con variables cuyo contenido es muy largo
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
