/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 12:24:26 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/10 13:02:11 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_unset(char **args, t_mini *mini)
{
	int	i;
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc == 1) //MIRAR
		return ;
	i = 1;
	while (args[i])
	{
		ft_unset_env(args[i], mini);
		i++;
	}
}