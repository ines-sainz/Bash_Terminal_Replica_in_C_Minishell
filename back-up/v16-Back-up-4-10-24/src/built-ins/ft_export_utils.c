/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/01 08:55:10 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_first_letter(char **args, t_mini *mini, int *i)
{
	if ((ft_isalpha(args[*i][0]) == 0 || args[*i][0] == '=')
		&& args[*i][0] != '_')
	{
		printf("minishell: export: `%s': not a valid identifier\n", args[*i]);
		ft_export_env("?=1", mini);
		(*i)++;
		return (ERR);
	}
	return (OK);
}

int	find_equal(char **args, int *i, int *j)
{
	*j = -1;
	while (args[*i][++(*j)])
	{
		if (args[*i][*j] == '=')
			return (*j);
	}
	return (0);
}

void	not_equal_case(char **args, int *i, t_mini *mini)
{
	int	j;

	j = 1;
	while (args[*i][j])
	{
		if (ft_isalnum(args[*i][j]) == 0 && args[*i][j] != '_')
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				args[*i]);
			ft_export_env("?=1", mini);
			break ;
		}
		j++;
	}
	if (j == (int)ft_strlen(args[*i]))
	{
		ft_export_env(args[*i], mini);
		ft_export_env("?=0", mini);
	}
}

void	equal_case(char **args, int *i, t_mini *mini, int *pos_equal)
{
	int	j;

	j = 1;
	while (args[*i][j] && j < *pos_equal)
	{
		if (ft_isalnum(args[*i][j]) == 0 && args[*i][j] != '_')
		{
			printf("minishell: export: `%s': not a valid identifier\n",
				args[*i]);
			ft_export_env("?=1", mini);
			break ;
		}
		j++;
	}
	if (j == *pos_equal)
	{
		ft_export_env(args[*i], mini);
		ft_export_env("?=0", mini);
	}
}

void	export_args(char **args, t_mini *mini)
{
	int	i;
	int	j;
	int	pos_equal;
	int	void_arg;

	i = 1;
	j = 0;
	pos_equal = 0;
	void_arg = 0;
	while (args[i])
	{
		if (args[i][0] != '\0')
		{
			if (handle_first_letter(args, mini, &i) == ERR)
				continue ;
			pos_equal = find_equal(args, &i, &j);
			j = 1;
			if (pos_equal == 0)
				not_equal_case(args, &i, mini);
			else
				equal_case(args, &i, mini, &pos_equal);
		}
		else
		{
			ft_dprintf(2, "minishell: export: `': not a valid identifier\n");
			void_arg++;
		}
		i++;
	}
	if (void_arg != 0)
		ft_export_env("?=1", mini);
}
