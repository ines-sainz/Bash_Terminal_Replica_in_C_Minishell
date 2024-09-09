/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:19 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/09 19:00:44 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		printf("echo devuelve:\n"); //Eliminar
		printf("\n");
		return ;
	}
	i = 1;
	n_flag = 0;
	detect_and_count_flag(args, &i, &n_flag);
	printf("echo devuelve: "); //Eliminar
	if ((n_flag + 1) == argc)
		return ;
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i][0] != '\0' && i != argc - 1)
			printf(" ");
		i++;
	}
	if (n_flag == 0)
		printf("\n");
}
