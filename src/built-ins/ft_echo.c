/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:28:19 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/05 14:37:31 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_echo(t_args *args)
{
	int	i;
	int	j;
	int	flag;
	int	argc;
	int	n_flag;

	flag = 0;
	argc = 0;
	n_flag = 0;
	i = 0;
	while (args->args[i++])
		argc++;
	printf("echo argc = %i\n", argc);
	if (argc == 1)
	{
		printf("echo devuelve:\n");
		printf("\n");
		return ;
	}
	i = 1;
	while (args->args[i] && ft_strlen(args->args[i]) >= 2)
	{
		if (args->args[i][0] != '-')
			break ;
		else
		{
			j = 1;
			while (args->args[i][j])
			{
				if (args->args[i][j] != 'n')
					flag++;
				j++;
			}
			if (flag == 0)
				n_flag++;
			else
				break ;
		}
		i++;
	}
	printf("echo devuelve: ");
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
