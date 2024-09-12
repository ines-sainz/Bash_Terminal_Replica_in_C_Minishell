/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:29:43 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/12 13:52:11 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_pwd(char **args)
{
	char	*cwd;
	int		i;

	i = 0;
	while (args[i])
		i++;
	if (i > 1)
		return ;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
}