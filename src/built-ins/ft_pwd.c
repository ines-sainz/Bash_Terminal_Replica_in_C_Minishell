/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:29:43 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/12 15:01:13 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_pwd(char **args, t_mini *mini)
{
	char	*cwd;
	int		argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 1)
		return ;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (ft_find_env(mini, "PWD"))
		{
			cwd = ft_strdup(ft_find_env(mini, "PWD"));
			printf("%s\n", cwd);
			free (cwd);
			return ;
		}
		perror("minishell: getcwd");
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
}
