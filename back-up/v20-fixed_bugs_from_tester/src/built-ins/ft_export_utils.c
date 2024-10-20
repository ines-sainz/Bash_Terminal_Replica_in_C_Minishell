/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/10/20 14:46:34 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_first_letter(char **args, int *i, int *exit_status)
{
	if ((ft_isalpha(args[*i][0]) == 0 || args[*i][0] == '=')
		&& args[*i][0] != '_')
	{
		ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
			args[*i]);
		*exit_status = 1;
		(*i)++;
		return (ERR);
	}
	return (OK);
}

static int	find_equal(char **args, int *i, int *j)
{
	*j = -1;
	while (args[*i][++(*j)])
	{
		if (args[*i][*j] == '=')
			return (*j);
	}
	return (0);
}

static void	not_equal_case(char **args, int *i, t_mini *mini, int *exit_status)
{
	int	j;

	j = 1;
	while (args[*i][j])
	{
		if (ft_isalnum(args[*i][j]) == 0 && args[*i][j] != '_')
		{
			ft_dprintf(2, "minishell: export: `%s': not a valid identifier\n",
				args[*i]);
			*exit_status = 1;
			break ;
		}
		j++;
	}
	if (j == (int)ft_strlen(args[*i]))
	{
		ft_export_env(args[*i], mini);
		*exit_status = 0;
	}
}

static int	equal_case(char **args, int *i, t_mini *mini, int *pos_equal)
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

void	export_args(char **args, t_mini *mini, int *exit_status)
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
			if (handle_first_letter(args, &i, exit_status) == ERR)
				continue ;
			pos_equal = find_equal(args, &i, &j);
			j = 1;
			if (pos_equal == 0)
				not_equal_case(args, &i, mini, exit_status);
			else
				*exit_status = equal_case(args, &i, mini, &pos_equal);
		}
		else
		{
			ft_dprintf(2, "minishell: export: `': not a valid identifier\n");
			void_arg++;
		}
		i++;
	}
	if (void_arg != 0)
		*exit_status = 1;
}
