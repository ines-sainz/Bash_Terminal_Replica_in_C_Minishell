/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:19 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/23 14:24:52 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_echo_args(char **args, int i, int argc)
{
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i][0] != '\0' && i != argc - 1)
			printf(" ");
		i++;
	}
}

static void	detect_and_count_flag(char **args, int *i, int	*n_flag)
{
	int	flag;
	int	j;

	flag = 0;
	while (args[*i] && ft_strlen(args[*i]) >= 2)
	{
		if (args[*i][0] != '-')
			break ;
		else
		{
			j = 1;
			while (args[*i][j])
			{
				if (args[*i][j] != 'n')
					flag++;
				j++;
			}
			if (flag == 0)
				(*n_flag)++;
			else
				break ;
		}
		(*i)++;
	}
}

void	ft_built_echo(char **args)
{
	int	i;
	int	argc;
	int	n_flag;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1)
	{
		printf("DB: echo devuelve:\n"); //Eliminar
		printf("\n");
		return ;
	}
	i = 1;
	n_flag = 0;
	detect_and_count_flag(args, &i, &n_flag);
	printf("DB: echo devuelve: "); //Eliminar
	if ((n_flag + 1) == argc)
		return ;
	print_echo_args(args, i, argc);
	if (n_flag == 0)
		printf("\n");
}
