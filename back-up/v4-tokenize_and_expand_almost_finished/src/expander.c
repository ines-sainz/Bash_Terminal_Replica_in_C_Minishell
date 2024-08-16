/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:07:57 by isainz-r          #+#    #+#             */
/*   Updated: 2024/08/16 22:11:00 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Sólo entra aquí el dolar y la palabra de después $....
/* char	*expander(char *variable)
{
	char	*expanded;

	if (variable[1] >= '0' && variable[1] <= '9')
		return (ft_substr(variable, 2, (ft_strlen(variable) - 2)));
	expanded = getenv(++variable);
	if (!expanded)
		return (ft_strdup("\n"));
	return (expanded);
} */

char *expander(char *input, t_bool started_in_single_quote)
{
	// Variables para manejar los nombres de variables de entorno, sus valores y el resultado
	char	*var_name;
	char	*env_value;
	char	*result = NULL;
	// Capacidad inicial del resultado y contadores para las posiciones de entrada y salida
	size_t	result_capacity;
	size_t	i;
	size_t	j;
	// Buffer temporal para re-asignar memoria si es necesario
	char	*new_result;
	// Variables de estado para manejar si estamos dentro de comillas simples o dobles
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	// Variable para manejar el siguiente carácter y el inicio del nombre de variable
	char	next_char;
	int		start;
	// Longitud del valor de la variable de entorno
	size_t	env_len;

	// Inicializamos la capacidad del resultado como el doble de la longitud de la entrada
	result_capacity = ft_strlen(input) * 2;
	result = malloc(result_capacity); // Reservamos memoria para el resultado
	if (!result) // Si la memoria no se pudo asignar, devolvemos NULL
		return (NULL);
	// Iniciamos el estado de comillas simples según el parámetro de entrada y las comillas dobles en falso
	in_single_quotes = started_in_single_quote;
	in_double_quotes = false;
	// Inicializamos los contadores para la entrada (i) y el resultado (j)
	i = 0;
	j = 0;
	while (input[i]) // Iteramos a través de la cadena de entrada
	{
		// Si la capacidad del resultado se excede, la duplicamos y reasignamos memoria
		if (j >= result_capacity - 1)
		{
			result_capacity *= 2;
			new_result = malloc(result_capacity);
			if (!new_result)
			{
				free(result); // Liberamos la memoria del resultado anterior en caso de error
				return (NULL);
			}
			ft_memcpy(new_result, result, j); // Copiamos el resultado anterior en el nuevo buffer
			free(result);
			result = new_result; // Actualizamos el puntero del resultado
		}
		// Manejo de comillas simples fuera de comillas dobles
		if (input[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes; // Alternamos el estado de comillas simples
			i++; // Saltamos la comilla simple
			continue ;
		}
		// Manejo de comillas dobles fuera de comillas simples
		else if (input[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes; // Alternamos el estado de comillas dobles
			i++; // Saltamos la comilla doble
			continue ;
		}
		// Manejo de la expansión de variables con el símbolo `$`
		if (input[i] == '$' && !in_single_quotes)
		{
			i++; // Avanzamos para verificar el carácter siguiente
			// Si el símbolo `$` es el último carácter, lo añadimos tal cual al resultado
			if (input[i] == '\0') 
			{
				result[j++] = '$';
				continue ;
			}
			// Obtenemos el siguiente carácter
			next_char = input[i];
			// Si el siguiente carácter no es alfabético ni un guion bajo, tratamos `$` como un literal
			if (!ft_isalpha(next_char) && next_char != '_')
			{
				result[j++] = '$'; // Copiamos el símbolo `$`
				result[j++] = next_char; // Copiamos el carácter especial
				i++;
				continue ;
			}
			start = i; // Guardamos el inicio del nombre de la variable
			// Avanzamos a través del nombre de la variable (alfabético o guión bajo)
			while (input[i] && (ft_isalpha(input[i]) || input[i] == '_'))
				i++;
			var_name = ft_substr(input, start, i - start); // Extraemos el nombre de la variable desde la entrada
			env_value = getenv(var_name); // Obtenemos el valor de la variable de entorno
			free(var_name); // Liberamos la memoria utilizada por el nombre de la variable
			// Si se encontró el valor de la variable de entorno, lo añadimos al resultado
			if (env_value)
			{
				env_len = ft_strlen(env_value);
				// Si la capacidad actual no es suficiente, la aumentamos
				if (j + env_len >= result_capacity - 1)
				{
					result_capacity = (j + env_len) * 2;
					new_result = malloc(result_capacity);
					if (!new_result)
					{
						free(result); // Liberamos la memoria si hay un error en la asignación
						return (NULL);
					}
					ft_memcpy(new_result, result, j); // Copiamos el resultado anterior al nuevo buffer
					free(result);
					result = new_result;
				}
				ft_strcpy(result + j, env_value); // Copiamos el valor de la variable de entorno al resultado
				j += env_len; // Avanzamos el índice del resultado
			}
			continue ; // Evitamos un incremento doble de `i`
		}

		// Copiamos el carácter actual al resultado (excepto las comillas)
		result[j++] = input[i++];
	}
	result[j] = '\0'; // Añadimos el carácter nulo al final del resultado para marcar el fin de la cadena
	return (result); // Devolvemos el resultado expandido
}

//Funciona bien pero con error con variables cuyo contenido es muy largo
/* char *expander(char *input, bool started_in_single_quote)
{
    char *var_name;
    char *env_value;
    char *result;
    int i, j;
    bool in_single_quotes = started_in_single_quote;
    bool in_double_quotes = false;

    result = malloc(strlen(input) * 2); // Allocate more space for potential expansions
    i = 0;
    j = 0;

    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quotes)
            in_single_quotes = !in_single_quotes;
        else if (input[i] == '"' && !in_single_quotes)
            in_double_quotes = !in_double_quotes;
        
        if (input[i] == '$' && !in_single_quotes && (i == 0 || input[i-1] != '\\'))
        {
            i++;
            int start = i;
            while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
                i++;
            var_name = ft_substr(input, start, i - start);
            env_value = getenv(var_name);
            free(var_name);

            if (env_value)
            {
                strcpy(result + j, env_value);
                j += strlen(env_value);
            }
            // Preserve the space after variable expansion if it exists
            if (input[i] == ' ' && in_double_quotes)
            {
                result[j++] = ' ';
                i++;
            }
        }
        else if (input[i] != '\'' && input[i] != '"')
        {
            result[j++] = input[i++];
        }
        else
        {
            i++; // Skip quotes
        }
    }
    result[j] = '\0';

    return result;
} */

/* char *expander(char *input)
{
	char *var_name;

	if (ft_isspace(*input))
		return (ft_strdup(input));
	if (*input == '$')
	{
		var_name = ft_strndup(input + 1, ft_strlen(input));
		// Comprueba si la variable de entorno existe
		if (getenv(var_name) != NULL)
		{
			free(var_name);
			return getenv(var_name);
		}
		free(var_name);
	}
	else
		return (ft_strdup(input));
	return (NULL);
} */
