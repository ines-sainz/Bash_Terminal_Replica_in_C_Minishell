/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:40:06 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/13 23:59:26 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_number(const char *str, long long int *result)
{
	int					negative;
	unsigned long long	value;
	int					digit;

	negative = 0;
	value = 0;
	if (*str == '-')
	{
		negative = 1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str)
	{
		if (!isdigit(*str))
			return ERR; // No es un número válido
		digit = *str - '0';
		// Comprobar desbordamiento
		if (value > LLONG_MAX / 10
			|| (value == LLONG_MAX / 10 && digit > LLONG_MAX % 10 + negative))
				return (ERR); // Desbordamiento
		value = value * 10 + digit;
		str++;
	}
	// Aplicar signo
	if (negative)
	{
		if (value > (unsigned long long)LLONG_MAX + 1)
			return (ERR);
		*result = -(long long)value;
	}
	else
	{
		if (value > LLONG_MAX)
			return (ERR); // Fuera de rango para positivos
		*result = (long long)value;
	}
	return (OK); // Éxito
}

static int	its_only_numbers(char *str)
{
	int			i;
	long long	num_not_valid;

	i = 0;
	if (*str == '\0')
		return (ERR);
	while (str[i] != '\0')
	{
		if (i == 0 && str[i] == '-')
			i++;
		if (!ft_isdigit(str[i]))
			return (ERR);
		i++;
	}
	// LLONG_MAX 9223372036854775808 LLONG_MIN -9223372036854775809
	if (is_valid_number(str, &num_not_valid) == ERR)
		return (ERR);
	/* num_not_valid = ft_custom_atoll(str);
	if ((unsigned long long int)num_not_valid == ULLONG_MAX)
		return (ERR); */
	return (OK);
}

static int	multiple_args(t_args *args, char **exit_args, int argc)
{
	long long	nbr_atoll;
	int			modulus;

	(void)args;
	if (its_only_numbers(exit_args[1]) == ERR)
	{
		printf("minishell: exit: %s: numeric argument required\n",
			exit_args[1]);
		//free_at_exit(args);
		//exit(2);
		return (2);
	}
	else if (its_only_numbers(exit_args[1]) == OK && argc == 2)
	{
		is_valid_number(exit_args[1], &nbr_atoll);
		//nbr_atoi = ft_atoi(exit_args[1]);
		modulus = (nbr_atoll % 256);
		//free_at_exit(args);
		//exit(modulus);
		return (modulus);
	}
	else if (its_only_numbers(exit_args[1]) == OK && argc > 2)
		return (-1);
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
		//free_at_exit(args);
		//exit(0);
		return (0);
	}
	if (argc >= 2)
		exit_status = multiple_args(args, exit_args, argc);
	return (exit_status);
}
