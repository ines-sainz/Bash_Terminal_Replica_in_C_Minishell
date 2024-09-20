/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:40:06 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/20 11:15:00 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	its_only_numbers(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (i == 0 && str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
			return (ERR);
		i++;
	}
	return (OK);
}

static void	multiple_args(t_args *args, char **exit_args, int argc, t_mini *mini)
{
	int	nbr_atoi;
	int	modulus;

	if (its_only_numbers(exit_args[1]) == ERR)
	{
		printf("minishell: exit: %s: numeric argument required\n",
			exit_args[1]);
		free_at_exit(args);
		exit(2);
	}
	else if (its_only_numbers(exit_args[1]) == OK && argc == 2)
	{
		nbr_atoi = ft_atoi(exit_args[1]);
		modulus = (nbr_atoi % 256);
		free_at_exit(args);
		exit(modulus);
	}
	else if (its_only_numbers(exit_args[1]) == OK && argc > 2)
		ft_export_env("?=1", mini);
}

void	ft_built_exit(t_args *args, char **exit_args, t_mini *mini)
{
	int	argc;

	argc = 0;
	while (exit_args[argc])
		argc++;
	printf("exit argc = %i\n", argc);
	if (argc == 1)
	{
		free_at_exit(args);
		exit(0);
	}
	if (argc >= 2)
		multiple_args(args, exit_args, argc, mini);
}
