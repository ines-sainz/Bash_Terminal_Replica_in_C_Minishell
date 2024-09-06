/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 14:40:06 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/06 15:03:00 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_exit(t_args *args)
{
	int	i;
	int	argc;

	argc = 0;
	i = 0;
	while (args->args[i++])
		argc++;
	printf("exit argc = %i\n", argc);
	if (argc == 1)
	{
		free_at_exit(args);
		exit(0);
	}
	if (argc >= 2)
	{
		i = 0;
		while (args->args[1][i])
		{
			if (ft_isdigit(args->args[1][i]) == 0) // Not Digit detected
			{
				free_at_exit(args);
				exit(2);
			}
			i++;
		}
	}
	// free_at_exit(args);
	// exit(0);
}