/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/16 20:08:52 by danjimen         ###   ########.fr       */
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

	// Asignamos el puntero de entrada al inicio de la cadena de entrada
	input_ptr = args->input;
	// Iteramos sobre cada carácter de la cadena de entrada
	while (*input_ptr)
	{
		// Verificamos si encontramos una comilla simple y no estamos dentro de una comilla doble
		if (*input_ptr == '\'' && !args->in_double_quote)
			args->in_single_quote = !args->in_single_quote; // Alternamos el estado de la bandera de comilla simple
		// Verificamos si encontramos una comilla doble y no estamos dentro de una comilla simple
		else if (*input_ptr == '\"' && !args->in_single_quote)
			args->in_double_quote = !args->in_double_quote; // Alternamos el estado de la bandera de comilla doble
		// Si encontramos un espacio o un pipe ('|'), y no estamos dentro de comillas.
		if ((ft_isspace(*input_ptr) || *input_ptr == '|') && !args->in_single_quote && !args->in_double_quote)
		{
			// Si `arg_ptr` ha avanzado desde su posición inicial, es decir, si hemos acumulado un argumento
			if (*arg_ptr != args->arg)
			{
				**arg_ptr = '\0'; // Termina la cadena actual con un carácter nulo
				// Expande el argumento si es necesario (por ejemplo, manejo de variables)
				expanded_arg = expander(args->arg, args->in_single_quote);
				// Si la expansión fue exitosa, añadimos el argumento expandido al arreglo de argumentos
				if (expanded_arg)
					args->args[(*argc)++] = expanded_arg;
				// Reiniciamos `arg_ptr` a la posición inicial para el próximo argumento
				*arg_ptr = args->arg;
			}
			// Si encontramos un pipe ('|'), lo añadimos como un argumento separado
			if (*input_ptr == '|')
				args->args[(*argc)++] = ft_strdup("|");
		}
		// Manejo del símbolo de dólar ('$') fuera de comillas simples
		else if (*input_ptr == '$' && !args->in_single_quote)
		{
			// Check if the next character is a digit
			next_char = input_ptr + 1;
			/* if (next_char == '$')
			{
				// // Copy $ as literal
				// *(*arg_ptr)++ = *input_ptr++;
				// // Skip the digits
				// while (isdigit(*temp_ptr))
				next_char++;
				// Continue copying the rest of the string
				input_ptr = next_char;
				continue;
			} */
			// Verificamos si el carácter siguiente es un dígito o un asterisco ('*')
			if (isdigit(*next_char) || *next_char == '*')
			{
				// // Copy $ as literal
				// *(*arg_ptr)++ = *input_ptr++;
				// // Skip the digits
				// while (isdigit(*temp_ptr))
				next_char++;
				// Continuamos con el siguiente carácter sin copiar el '$' actual
				input_ptr = next_char;
				continue; // Salta a la siguiente iteración
			}
			/* else if (!ft_isalpha(*next_char) && *next_char != '_')
			{
				// Copy $ as literal
				*(*arg_ptr)++ = '$';
				input_ptr++;
				continue;
			} */
			else
				*(*arg_ptr)++ = *input_ptr; // Copiamos el '$' en el argumento
		}
		else
			*(*arg_ptr)++ = *input_ptr; // Copiamos el carácter actual en el argumento
		input_ptr++; // Avanzamos al siguiente carácter de la cadena de entrada
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
	
	// Si `arg_ptr` ha avanzado desde su posición inicial, añadimos el último argumento
	if (*arg_ptr != args->arg)
	{
		**arg_ptr = '\0'; // Termina la cadena actual con un carácter nulo
		expanded_arg = expander(args->arg, args->in_single_quote); // Expande el argumento si es necesario
		if (expanded_arg)
			args->args[(*argc)++] = expanded_arg; // Añade el argumento expandido al arreglo de argumentos
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
