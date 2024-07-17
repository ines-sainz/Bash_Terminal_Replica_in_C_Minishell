/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 12:49:02 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/17 08:57:00 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	ft_count_elements(const char *s, char c)
{
	int		count;
	int		i;
	int		sim_quote;
	int		dbl_quote;
	int		flag;

	count = 0;
	i = 0;
	sim_quote = 0;
	dbl_quote = 0;
	flag = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		if (s[i] == '\'')
			sim_quote = 1;
		else if (s[i] == '\"')
			dbl_quote = 1;
		while ((s[i] != c || sim_quote == 1 || dbl_quote == 1) && s[i] != '\0')
		{
			i++;
			if ((s[i] == '\'' && sim_quote == 1 && s[i + 1] == c))
			{
				if (flag != 0)
					flag--;
				else
					sim_quote = 0;
			}
			else if ((s[i] == '\'' && sim_quote == 1 && s[i + 1] != c))
				flag++;
			if ((s[i] == '\"' && dbl_quote == 1 && s[i + 1] == c))
			{
				if (flag != 0)
					flag--;
				else
					dbl_quote = 0;
			}
			else if ((s[i] == '\"' && dbl_quote == 1 && s[i + 1] != c))
				flag++;
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

static int	find_string(int *i, char c, const char *s, int *flag)
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
	if (s[*i] == '\'')
		sim_quote = 1;
	else if (s[*i] == '\"')
		dbl_quote = 1;
	while ((s[*i] != c || sim_quote == 1 || dbl_quote == 1) && s[*i] != '\0')
	{
		(*i)++;
		if ((s[*i] == '\'' && sim_quote == 1 && s[*i + 1] == c))
		{
			if (*flag != 0)
				(*flag)--;
			else
				sim_quote = 0;
		}
		else if ((s[*i] == '\'' && sim_quote == 1 && s[*i + 1] != c))
			(*flag)++;
		if ((s[*i] == '\"' && dbl_quote == 1 && s[*i + 1] == c))
		{
			if (*flag != 0)
				(*flag)--;
			else
				dbl_quote = 0;
		}
		else if ((s[*i] == '\"' && dbl_quote == 1 && s[*i + 1] != c))
			(*flag)++;
	}
	if (sim_quote == 1 || dbl_quote == 1)
		(*i)++;
	return (start);
}

static char	**ft_copy_string(const char *s, char c, char **split)
{
	int	start;
	int	count;
	int	i;
	int	flag;

	start = 0;
	count = 0;
	i = 0;
	flag = 0;
	while (s[i])
	{
		start = find_string(&i, c, s, &flag);
		printf("flag == %i\n", flag);
		if (s[start] == '\'' || s[start] == '\"')
			start++;
		split[count] = (char *)malloc((i - start - flag) * sizeof(char));
		if (!split[count])
		{
			free_split_awk(split, count);
			return (NULL);
		}
		if (s[start] == '\'' || s[start] == '\"')
			ft_strlcpy(split[count++], s + start, i - start - flag);
		else
			ft_strlcpy(split[count++], s + start, i - start + 1  - flag);
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