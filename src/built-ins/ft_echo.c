/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:19 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/06 22:31:11 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	detect_and_count_flag(t_args *args, int *i, int	*n_flag)
{
	int	flag;
	int	j;

	flag = 0;
	while (args->args[*i] && ft_strlen(args->args[*i]) >= 2)
	{
		if (args->args[*i][0] != '-')
			break ;
		else
		{
			j = 1;
			while (args->args[*i][j])
			{
				if (args->args[*i][j] != 'n')
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

void	ft_built_echo(t_args *args)
{
	int	i;
	int	argc;
	int	n_flag;

	argc = 0;
	while (args->args[argc])
		argc++;
	if (argc == 1)
	{
		printf("echo devuelve:\n"); //Eliminar
		printf("\n");
		return ;
	}
	i = 1;
	n_flag = 0;
	detect_and_count_flag(args, &i, &n_flag);
	printf("echo devuelve: "); //Eliminar
	while (args->args[i])
	{
		printf("%s", args->args[i]);
		if (i != argc - 1)
			printf(" ");
		i++;
	}
	if (n_flag == 0)
		printf("\n");
}
