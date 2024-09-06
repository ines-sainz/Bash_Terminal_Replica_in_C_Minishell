/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:40:06 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/06 19:40:28 by danjimen         ###   ########.fr       */
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

static void	multiple_args(t_args *args, int argc, t_mini *mini)
{
	int	nbr_atoi;
	int	modulus;

	if (its_only_numbers(args->args[1]) == ERR)
	{
		free_at_exit(args);
		exit(2);
	}
	else if (its_only_numbers(args->args[1]) == OK && argc == 2)
	{
		nbr_atoi = ft_atoi(args->args[1]);
		modulus = (nbr_atoi % 256);
		free_at_exit(args);
		exit(modulus);
	}
	else if (its_only_numbers(args->args[1]) == OK && argc > 2)
		ft_export_env("?=1", mini);
}

void	ft_built_exit(t_args *args, t_mini *mini)
{
	int	argc;

	argc = 0;
	while (args->args[argc])
		argc++;
	printf("exit argc = %i\n", argc);
	if (argc == 1)
	{
		free_at_exit(args);
		exit(0);
	}
	if (argc >= 2)
		multiple_args(args, argc, mini);
}
