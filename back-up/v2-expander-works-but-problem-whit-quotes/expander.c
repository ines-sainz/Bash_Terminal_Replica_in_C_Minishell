/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:07:57 by isainz-r          #+#    #+#             */
/*   Updated: 2024/08/12 22:05:22 by danjimen         ###   ########.fr       */
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

char *expander(char *input, bool started_in_single_quote)
{
	char *var_name;
	char *env_value;
	char *result = NULL;
	size_t result_capacity = strlen(input) * 2;
	size_t i, j;
	bool in_single_quotes = started_in_single_quote;
	bool in_double_quotes = false;

	result = malloc(result_capacity);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (j >= result_capacity - 1)
		{
			result_capacity *= 2;
			char *new_result = malloc(result_capacity);
			if (!new_result)
			{
				free(result);
				return NULL;
			}
			ft_memcpy(new_result, result, j);
			free(result);
			result = new_result;
		}
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
				size_t env_len = ft_strlen(env_value);
				if (j + env_len >= result_capacity - 1)
				{
					result_capacity = (j + env_len) * 2;
					char *new_result = malloc(result_capacity);
					if (!new_result)
					{
						free(result);
						return NULL;
					}
					ft_memcpy(new_result, result, j);
					free(result);
					result = new_result;
				}
				ft_strcpy(result + j, env_value);
				j += env_len;
			}
			// Preserve the space after variable expansion if it exists
			if (input[i++] == ' ' && in_double_quotes)
				result[j++] = ' ';
		}
		else if (input[i] != '\'' && input[i] != '"')
			result[j++] = input[i++];
		else
			i++; // Skip quotes
	}
	result[j] = '\0';
	return (result);
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
