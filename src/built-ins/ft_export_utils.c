/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/20 23:06:35 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	handle_first_letter(char **args, int *i)
{
	if ((ft_isalpha(args[*i][0]) == 0 || args[*i][0] == '=')
		&& args[*i][0] != '_')
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
			args[*i]);
		return (1);
	}
	return (0);
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

int	not_equal_case(char **args, int *i, t_mini *mini)
{
	int	j;
	int	exit_status;

	j = 1;
	exit_status = 0;
	while (args[*i][j])
	{
		if (ft_isalnum(args[*i][j]) == 0 && args[*i][j] != '_')
		{
			ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				args[*i]);
			exit_status = 1;
			break ;
		}
		j++;
	}
	if (j == (int)ft_strlen(args[*i]))
	{
		ft_export_env(args[*i], mini);
		exit_status = 0;
	}
	return (exit_status);
}

int	equal_case(char **args, int *i, t_mini *mini, int *pos_equal)
{
	int	j;
	int	exit_status;

	j = 1;
	exit_status = 0;
	while (args[*i][j] && j < *pos_equal)
	{
		if (ft_isalnum(args[*i][j]) == 0 && args[*i][j] != '_')
		{
			ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				args[*i]);
			exit_status = 1;
			break ;
		}
		j++;
	}
	if (j == *pos_equal)
	{
		ft_export_env(args[*i], mini);
		exit_status = 0;
	}
	return (exit_status);
}

