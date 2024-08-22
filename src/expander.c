/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:07:57 by isainz-r          #+#    #+#             */
/*   Updated: 2024/08/22 21:31:55 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/* static int	pid_case(t_args *args, size_t *j, t_mini *mini)
{
	int		env_len;
	char	*new_result;

	if (ft_find_env(mini, "$"))
	{
		env_len = ft_strlen(ft_find_env(mini, "$"));
		if (*j + env_len >= args->result_capacity - 1)
		{
			args->result_capacity = (*j + env_len) * 2;
			new_result = malloc(args->result_capacity);
			if (!new_result)
			{
				free(args->result);
				args->result = NULL;
				return (1);
			}
			ft_memcpy(new_result, args->result, *j);
			free(args->result);
			args->result = NULL;
			args->result = new_result;
		}
		ft_strcpy(args->result + *j, ft_find_env(mini, "$"));
		*j += env_len;
	}
	return (0);  // Indica éxito
} */

/* static void	check_quotes(char *input, size_t *i, t_bool *in_single_quotes, t_bool *in_double_quotes)
{
	if (input[*i] == '\''  && !*in_double_quotes)
		*in_single_quotes = !*in_single_quotes;
	if (input[*i] == '"'  && !*in_single_quotes)
		*in_double_quotes = !*in_double_quotes;
	(*i)++; // Skip the quote
} */

/* static char	*expand_vars(t_args *args, size_t *i, size_t *j, t_mini *mini)
{
	char	*var_name;
	char	*env_value;
	char	*new_result;
	char	next_char;
	int		start;
	size_t	env_len;
	char	*input;
	char	*result;
	//size_t	result_capacity;

	input = args->arg;
	result = args->result;
	args->result_capacity = ft_strlen(input);
	if (input[*i + 1] == '$') // $$ = PID
	{
		if (pid_case(args, j, mini))
			return (NULL);  // Retorna NULL si hubo un error de memoria en pid_case
		*i += 2;
		result = args->result;  // Actualiza result después de la llamada a pid_case
	}
	else
	{
		(*i)++;
		if (input[*i] == '\0') // $ it's at the end of a string (or string part)
			result[(*j)++] = '$';
		else
		{
			//exp_normal_or_special(&result, i, j, result_capacity, args, mini);
			// Check if the next character is a special character or digit
			next_char = input[*i];
			if (!ft_isalpha(next_char) && next_char != '_')
			{
				result[(*j)++] = '$'; // Keep the $ symbol as a literal
				result[(*j)++] = next_char; // Keep the special character
				i++;
			}
			else
			{
				start = *i;
				while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
					(*i)++;
				var_name = ft_substr(input, start, *i - start);
				env_value = ft_find_env(mini, var_name);
				printf("env_value ==> %s\n", env_value);
				//env_value = getenv(var_name);
				free(var_name);
				if (env_value)
				{
					env_len = ft_strlen(env_value);
					if (*j + env_len >= args->result_capacity - 1)
					{
						args->result_capacity = (*j + env_len) * 2;
						new_result = malloc(args->result_capacity);
						if (!new_result)
						{
							free(result);
							return NULL;
						}
						ft_memcpy(new_result, result, *j);
						free(result);
						result = new_result;
					}
					ft_strcpy(result + *j, env_value);
					*j += env_len;
				}
			}
		}
	}
	return (result);
} */

/* char *expander(t_args *args, t_mini *mini)
{
	size_t	i;
	size_t	j;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;

	args->result = malloc(ft_strlen(args->arg));
	if (!args->result)
		return (NULL);
	in_single_quotes = args->in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (args->arg[i])
	{
		if ((args->arg[i] == '\'' && !in_double_quotes)
			|| (args->arg[i] == '"' && !in_single_quotes))
			check_quotes(args->arg, &i, &in_single_quotes, &in_double_quotes);
		else if (args->arg[i] == '$' && !in_single_quotes)
			args->result = expand_vars(args, &i, &j, mini);
		else // Copy character unless it's a quote
			args->result[j++] = args->arg[i++];
	}
	args->result[j] = '\0';
	return (args->result);
} */

// Funciona y está lista para ser troceada.
// Cambiado el prototipo y utilizadas result, result_capacity y mini
// dentro de la estructura t_args
char *expander(t_args *args, t_mini *mini)
{
	char	*var_name;
	char	*env_value;
	//char	*result = NULL;
	//size_t	result_capacity;
	size_t	i;
	size_t	j;
	char	*new_result;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	char	next_char;
	int		start;
	size_t	env_len;

	args->result_capacity = ft_strlen(args->arg);
	//result_capacity = ft_strlen(input) * 2;
	args->result = malloc(args->result_capacity);
	if (!args->result)
		return (NULL);
	in_single_quotes = args->in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (args->arg[i])
	{
		if (j >= args->result_capacity - 1)
		{
			args->result_capacity += 1;
			new_result = malloc(args->result_capacity);
			if (!new_result)
			{
				free(args->result);
				args->result = NULL;
				return (NULL);
			}
			ft_memcpy(new_result, args->result, j);
			free(args->result);
			args->result = new_result;
		}

		if (args->arg[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++; // Skip the single quote
		}
		else if (args->arg[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++; // Skip the double quote
		}
		else if (args->arg[i] == '$' && !in_single_quotes)
		{
			if (args->arg[i + 1] == '$') // $$ = PID
			{
				//env_value = ft_find_env(mini, "$");
				env_value = ft_itoa(getpid());
				printf("env_value ==> %s\n", env_value);
				//printf(">>pid = %s\n", env_value);
				if (env_value)
				{
					env_len = ft_strlen(env_value);
					if (j + env_len >= args->result_capacity - 1)
					{
						args->result_capacity = (j + env_len) + 1;
						new_result = malloc(args->result_capacity);
						if (!new_result)
						{
							free(args->result);
							args->result = NULL;
							return (NULL);
						}
						ft_memcpy(new_result, args->result, j);
						free(args->result);
						args->result = new_result;
					}
					ft_strcpy(args->result + j, env_value);
					j += env_len;
				}
				i += 2;
			}
			else
			{
				i++;
				if (args->arg[i] == '\0') 
					args->result[j++] = '$';
				else
				{
					// Check if the next character is a special character or digit
					next_char = args->arg[i];
					if (!ft_isalpha(next_char) && next_char != '_')
					{
						args->result[j++] = '$'; // Keep the $ symbol as a literal
						args->result[j++] = next_char; // Keep the special character
						i++;
					}
					else
					{
						start = i;
						while (args->arg[i] && (ft_isalnum(args->arg[i]) || args->arg[i] == '_'))
							i++;
						var_name = ft_substr(args->arg, start, i - start);
						env_value = ft_find_env(mini, var_name);
						printf("env_value ==> %s\n", env_value);
						//env_value = getenv(var_name);
						free(var_name);
						if (env_value)
						{
							env_len = ft_strlen(env_value);
							if (j + env_len >= args->result_capacity - 1)
							{
								args->result_capacity = (j + env_len) + 1;
								new_result = malloc(args->result_capacity);
								if (!new_result)
								{
									free(args->result);
									args->result = NULL;
									return (NULL);
								}
								ft_memcpy(new_result, args->result, j);
								free(args->result);
								args->result = new_result;
							}
							ft_strcpy(args->result + j, env_value);
							j += env_len;
						}
					}
				}
			}
		}
		else // Copy character unless it's a quote
			args->result[j++] = args->arg[i++];
	}
	args->result[j] = '\0';
	return (args->result);
}

// Funciona y está lista para ser troceada
/* char *expander(char *input, t_bool started_in_single_quote, t_mini *mini)
{
	char	*var_name;
	char	*env_value;
	char	*result = NULL;
	size_t	result_capacity;
	size_t	i;
	size_t	j;
	char	*new_result;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	char	next_char;
	int		start;
	size_t	env_len;

	result_capacity = ft_strlen(input);
	//result_capacity = ft_strlen(input) * 2;
	result = malloc(result_capacity);
	if (!result)
		return (NULL);
	in_single_quotes = started_in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (input[i])
	{
		if (j >= result_capacity - 1)
		{
			result_capacity += 1;
			new_result = malloc(result_capacity);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			ft_memcpy(new_result, result, j);
			free(result);
			result = new_result;
		}

		if (input[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++; // Skip the single quote
		}
		else if (input[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++; // Skip the double quote
		}
		else if (input[i] == '$' && !in_single_quotes)
		{
			if (input[i + 1] == '$') // $$ = PID
			{
				//env_value = ft_find_env(mini, "$");
				env_value = ft_itoa(getpid());
				printf("env_value ==> %s\n", env_value);
				//printf(">>pid = %s\n", env_value);
				if (env_value)
				{
					env_len = ft_strlen(env_value);
					if (j + env_len >= result_capacity - 1)
					{
						result_capacity = (j + env_len) + 1;
						new_result = malloc(result_capacity);
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
				i += 2;
			}
			else
			{
				i++;
				if (input[i] == '\0') 
					result[j++] = '$';
				else
				{
					// Check if the next character is a special character or digit
					next_char = input[i];
					if (!ft_isalpha(next_char) && next_char != '_')
					{
						result[j++] = '$'; // Keep the $ symbol as a literal
						result[j++] = next_char; // Keep the special character
						i++;
					}
					else
					{
						start = i;
						while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
							i++;
						var_name = ft_substr(input, start, i - start);
						env_value = ft_find_env(mini, var_name);
						printf("env_value ==> %s\n", env_value);
						//env_value = getenv(var_name);
						free(var_name);
						if (env_value)
						{
							env_len = ft_strlen(env_value);
							if (j + env_len >= result_capacity - 1)
							{
								result_capacity = (j + env_len) + 1;
								new_result = malloc(result_capacity);
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
					}
				}
			}
		}
		else // Copy character unless it's a quote
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
} */

// Funciona, pero debo eliminar los "continue;" para poder partir el código
/* char *expander(char *input, t_bool started_in_single_quote)
{
	char	*var_name;
	char	*env_value;
	char	*result = NULL;
	size_t	result_capacity;
	size_t	i;
	size_t	j;
	char	*new_result;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	char	next_char;
	int		start;
	size_t	env_len;

	result_capacity = ft_strlen(input) * 2;
	result = malloc(result_capacity);
	if (!result)
		return (NULL);
	in_single_quotes = started_in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (input[i])
	{
		if (j >= result_capacity - 1)
		{
			result_capacity *= 2;
			new_result = malloc(result_capacity);
			if (!new_result)
			{
				free(result);
				return (NULL);
			}
			ft_memcpy(new_result, result, j);
			free(result);
			result = new_result;
		}

		if (input[i] == '\'' && !in_double_quotes)
		{
			in_single_quotes = !in_single_quotes;
			i++; // Skip the single quote
			continue ;
		}
		else if (input[i] == '"' && !in_single_quotes)
		{
			in_double_quotes = !in_double_quotes;
			i++; // Skip the double quote
			continue ;
		}
		if (input[i] == '$' && !in_single_quotes)
		{
			if (input[i + 1] == '$') // $$ = PID
			{
				var_name = ft_substr(input, start, i - start);
				env_value = ft_itoa(getpid());
				//printf(">>pid = %s\n", env_value);
				free(var_name);
				if (env_value)
				{
					env_len = ft_strlen(env_value);
					if (j + env_len >= result_capacity - 1)
					{
						result_capacity = (j + env_len) * 2;
						new_result = malloc(result_capacity);
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
				i = i + 2;
				continue ;
			}
			i++;
			if (input[i] == '\0') 
			{
				result[j++] = '$';
				continue ;
			}
			// Check if the next character is a special character or digit
			next_char = input[i];
			if (!ft_isalpha(next_char) && next_char != '_')
			{
				result[j++] = '$'; // Keep the $ symbol as a literal
				result[j++] = next_char; // Keep the special character
				i++;
				continue ;
			}
			start = i;
			while (input[i] && (ft_isalpha(input[i]) || input[i] == '_'))
				i++;
			var_name = ft_substr(input, start, i - start);
			env_value = getenv(var_name);
			free(var_name);
			if (env_value)
			{
				env_len = ft_strlen(env_value);
				if (j + env_len >= result_capacity - 1)
				{
					result_capacity = (j + env_len) * 2;
					new_result = malloc(result_capacity);
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
			continue ; // Avoid double increment of i
		}

		// Copy character unless it's a quote
		result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
} */

//Funciona bien pero con error con variables cuyo contenido es muy largo
/* char *expander(char *input, bool started_in_single_quote)
{
	char *var_name;
	char *env_value;
	char *result;
	int i, j;
	bool in_single_quotes = started_in_single_quote;
	bool in_double_quotes = t_false;

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
