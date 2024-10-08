/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:40:06 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/03 21:12:56 by danjimen         ###   ########.fr       */
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

static int	multiple_args(t_args *args, char **exit_args, int argc)
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
		return (1);
		//ft_export_env("?=1", mini);
	return (0);
}

int	ft_built_exit(t_args *args, char **exit_args)
{
	int	argc;
	int	exit_status;

	argc = 0;
	exit_status = 0;
	while (exit_args[argc])
		argc++;
	printf("DB: exit argc = %i\n", argc);
	if (argc == 1)
	{
		free_at_exit(args);
		exit(0);
	}
	if (argc >= 2)
		exit_status = multiple_args(args, exit_args, argc);
	return (exit_status);
}
