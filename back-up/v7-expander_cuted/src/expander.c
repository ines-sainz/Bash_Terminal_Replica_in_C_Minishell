/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:07:57 by isainz-r          #+#    #+#             */
/*   Updated: 2024/08/23 20:20:04 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static char	*initilize_vars(t_args *args, t_bool *in_double_quotes, size_t *i, size_t *j)
{
	args->result_capacity = ft_strlen(args->arg) - 2 + 1;
	args->result = malloc(args->result_capacity);
	if (!args->result)
		return (NULL);
	*in_double_quotes = t_false;
	*i = 0;
	*j = 0;
	return (args->result);
}

static void	check_quotes(t_args *args, size_t *i, t_bool *in_double_quotes)
{
	if (args->arg[*i] == '\''  && !*in_double_quotes)
		args->in_single_quote = !args->in_single_quote;
	if (args->arg[*i] == '"'  && !args->in_single_quote)
		*in_double_quotes = !*in_double_quotes;
	(*i)++; // Skip the quote
}

static char	*realloc_plain_text(t_args *args, size_t *i, size_t *j)
{
	char	*new_result;
	
	if (*j >= args->result_capacity - 1)
	{
		//args->result_capacity += ft_strlen(args->arg) - i - 1;
		args->result_capacity += *i;
		new_result = malloc(args->result_capacity);
		if (!new_result)
		{
			free(args->result);
			return (NULL);
		}
		ft_memcpy(new_result, args->result, *j);
		free(args->result);
		args->result = new_result;
	}
	return (args->result);
}

static char	*pid_case(t_args *args, size_t *i, size_t *j, t_mini *mini)
{
	int		env_len;
	char	*new_result;

	if (ft_find_env(mini, "$"))
	{
		env_len = ft_strlen(ft_find_env(mini, "$"));
		if (*j + env_len >= args->result_capacity - 1)
		{
			args->result_capacity += env_len - 2;
			new_result = malloc(args->result_capacity);
			if (!new_result)
			{
				free(args->result);
				args->result = NULL;
				return (NULL);
			}
			ft_memcpy(new_result, args->result, *j);
			free(args->result);
			args->result = new_result;
		}
		ft_strcpy(args->result + *j, ft_find_env(mini, "$"));
		*j += env_len;
	}
	*i += 2;
	return (args->result);  // Indica éxito
}

static char	*valid_var(t_args *args, size_t *i, size_t *j, int *start)
{
	char	*env_value;
	char	*new_result;

	env_value = ft_find_env(args->mini, ft_substr(args->arg, *start, *i - *start));
	printf("env_value ==> %s\n", env_value);
	if (env_value)
	{
		if (*j + ft_strlen(env_value) >= args->result_capacity - 1)
		{
			args->result_capacity += ft_strlen(env_value) - 1;
			new_result = malloc(args->result_capacity);
			if (!new_result)
			{
				free(args->result);
				args->result = NULL;
				return (NULL);
			}
			ft_memcpy(new_result, args->result, *j);
			free(args->result);
			args->result = new_result;
		}
		ft_strcpy(args->result + *j, env_value);
		*j += ft_strlen(env_value);
	}
	return (args->result);
}

static char	*next_is_number_or_valid(t_args *args, size_t *i, size_t *j)
{
	char	next_char;
	int		start;

	// Check if the next character is a special character or digit
	next_char = args->arg[*i];
	if (!ft_isalpha(next_char) && next_char != '_')
	{
		args->result[(*j)++] = '$'; // Keep the $ symbol as a literal
		args->result[(*j)++] = next_char; // Keep the special character
		(*i)++;
	}
	else
	{
		start = *i;
		while (args->arg[*i] && (ft_isalnum(args->arg[*i]) || args->arg[*i] == '_'))
			(*i)++;
		if (!valid_var(args, i, j, &start))
			return (NULL);
	}
	return (args->result);
}

static char *expand_vars(t_args *args, size_t *i, size_t *j, t_mini *mini)
{
	if (args->arg[*i + 1] == '$') // $$ = PID
	{
		if (!pid_case(args, i, j, mini))
			return (NULL);
	}
	else
	{
		(*i)++;
		if (args->arg[*i] == '\0' || (args->arg[*i] == '"' && args->arg[*i + 1] == '\0'))
			args->result[(*j)++] = '$'; // $ at the end of string
		else
		{
			if (!next_is_number_or_valid(args, i, j))
				return (NULL);
		}
	}
	return (args->result);
}

// Copia de la de debajo pero lista para ser cortada
char *expander(t_args *args, t_mini *mini)
{
	size_t	i;
	size_t	j;
	t_bool	in_double_quotes;

	if (!initilize_vars(args, &in_double_quotes, &i, &j))
		return (NULL);
	while (args->arg[i])
	{
		if (!realloc_plain_text(args, &i, &j))
			return (NULL);
		if ((args->arg[i] == '\'' && !in_double_quotes)
			|| (args->arg[i] == '"' && !args->in_single_quote))
			check_quotes(args, &i, &in_double_quotes);
		else if (args->arg[i] == '$' && !args->in_single_quote)
		{
			if (!expand_vars(args, &i, &j, mini))
				return (NULL);
		}
		else // Copy character unless it's a quote
			args->result[j++] = args->arg[i++];
	}
	args->result[j] = '\0';
	return (args->result);
}

// Funciona y está lista para ser troceada.
// Cambiado el prototipo y utilizadas result, result_capacity y mini
// dentro de la estructura t_args
/* char *expander(t_args *args, t_mini *mini)
{
	size_t	i;
	size_t	j;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	char	*var_name;
	char	*env_value;
	char	*new_result;
	char	next_char;
	int		start;
	size_t	env_len;

	//env_len = 0;
	args->result_capacity = ft_strlen(args->arg) - 2 + 1;
	args->result = malloc(args->result_capacity);
	if (!args->result)
		return (NULL);
	in_single_quotes = args->in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (args->arg[i])
	{
		printf("j => %li\n", j);
		printf("args->result_capacity => %li\n", args->result_capacity);
		if (j >= args->result_capacity - 1)
		{
			//args->result_capacity += ft_strlen(args->arg) - i - 1;
			args->result_capacity += i;
			new_result = malloc(args->result_capacity);
			if (!new_result)
			{
				free(args->result);
				return (NULL);
			}
			ft_memcpy(new_result, args->result, j);
			free(args->result);
			args->result = new_result;
		}
		if ((args->arg[i] == '\'' && !in_double_quotes)
			|| (args->arg[i] == '"' && !in_single_quotes))
			check_quotes(args, &i, &in_single_quotes, &in_double_quotes);
		// else if (args->arg[i] == '$' && !in_single_quotes)
		// 	args->result = expand_vars(args, &i, &j, mini);
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
						args->result_capacity += env_len - 2;
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
				//if (args->arg[i] == '\0')
				if (args->arg[i] == '\0' || (args->arg[i] == '"' && args->arg[i + 1] == '\0'))
					args->result[j++] = '$';
				else
				{
					// Check if the next character is a special character or digit
					next_char = args->arg[i];
					if (!ft_isalpha(next_char) && next_char != '_')
					{
						args->result[j++] = '$'; // Keep the $ symbol as a literal
						//if (!in_double_quotes)
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
								args->result_capacity += env_len - 1;
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
	printf("j => %li\n", j);
	printf("i => %li\n", i);
	printf("args->result_capacity => %li\n", args->result_capacity);
	return (args->result);
} */

// Funciona y está lista para ser troceada.
// Cambiado el prototipo y utilizadas result, result_capacity y mini
// dentro de la estructura t_args
/* char *expander(t_args *args, t_mini *mini)
{
	char	*var_name;
	char	*env_value;
	size_t	i;
	size_t	j;
	char	*new_result;
	t_bool	in_single_quotes;
	t_bool	in_double_quotes;
	char	next_char;
	int		start;
	size_t	env_len;

	args->result_capacity = ft_strlen(args->arg);
	args->result = malloc(args->result_capacity);
	if (!args->result)
		return (NULL);
	in_single_quotes = args->in_single_quote;
	in_double_quotes = t_false;
	i = 0;
	j = 0;
	while (args->arg[i])
	{
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
				//if (args->arg[i] == '\0')
				if (args->arg[i] == '\0' || (args->arg[i] == '"' && args->arg[i + 1] == '\0'))
					args->result[j++] = '$';
				else
				{
					// Check if the next character is a special character or digit
					next_char = args->arg[i];
					if (!ft_isalpha(next_char) && next_char != '_')
					{
						args->result[j++] = '$'; // Keep the $ symbol as a literal
						//if (!in_double_quotes)
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
} */
