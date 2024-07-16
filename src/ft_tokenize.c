/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/16 09:23:00 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_count_elements(const char *s, char c)
{
	int		count;
	int		i;
	int		sim_quote;
	int		dbl_quote;

	count = 0;
	i = 0;
	sim_quote = 0;
	dbl_quote = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		if (s[i] == '\'' && sim_quote == 0)
			sim_quote = 1;
		if (s[i] == '\"' && dbl_quote == 0)
			dbl_quote = 1;
		while ((s[i] != c || sim_quote == 1 || dbl_quote == 1) && s[i] != '\0')
		{
			i++;
			if ((s[i] == '\'' && sim_quote == 1))
				sim_quote = 0;
			if ((s[i] == '\"' && dbl_quote == 1))
				dbl_quote = 0;
		}
	}
	printf("palabras = %i\n", count);
	return (count);
}

static void	free_split_awk(char **split, int count)
{
	while (count > 0)
	{
		count--;
		free(split[count]);
	}
}

static int	find_string(int *i, char c, const char *s)
{
	int	start;
	int	sim_quote;
	int	dbl_quote;

	start = 0;
	sim_quote = 0;
	dbl_quote = 0;
	while (s[*i] == c)
		(*i)++;
	if (s[*i] == '\0')
		return (start);
	start = *i;
	if (s[*i] == '\'' && sim_quote == 0)
		sim_quote = 1;
	else if (s[*i] == '\"' && dbl_quote == 0)
		dbl_quote = 1;
	while (((s[*i] != c && s[*i] != '\0') || sim_quote == 1 || dbl_quote == 1))
	{
		(*i)++;
		if (s[*i] == '\'' && sim_quote == 1)
			sim_quote = 0;
		if ((s[*i] == '\"' && dbl_quote == 1))
			dbl_quote = 0;
	}
	return (start);
}

static char	**ft_copy_string(const char *s, char c, char **split)
{
	int	start;
	int	count;
	int	i;

	start = 0;
	count = 0;
	i = 0;
	while (s[i])
	{
		start = find_string(&i, c, s);
		if (s[start] == '\'' || s[start] == '\"')
			start++;
		split[count] = (char *)malloc((i - start) * sizeof(char));
		if (!split[count])
		{
			free_split_awk(split, count);
			return (NULL);
		}
		if (s[start - 1] == '\'' || s[start - 1] == '\"')
			ft_strlcpy(split[count++], s + start, i - start);
		else
			ft_strlcpy(split[count++], s + start, i - start + 1);
	}
	split[count] = NULL;
	return (split);
}

char	**ft_tokenize(char const *s, char c)
{
	char	**split;
	int		count;

	if (s == NULL)
		return (NULL);
	count = ft_count_elements(s, c);
	split = (char **)malloc((count + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	if (!ft_copy_string(s, c, split))
	{
		free(split);
		return (NULL);
	}
	return (split);
}