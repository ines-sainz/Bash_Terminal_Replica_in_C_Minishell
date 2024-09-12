/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:20:47 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/12 14:25:26 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_cd(char **args, t_mini *mini)
{
	char	*output;
	//char	*buffer;
	char	*temp;
	int		argc;

	//buffer = NULL;
	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
		printf("minishell: cd: too many arguments\n");
	ft_export_env(ft_strjoin("OLDPWD=", ft_get_env("PWD", mini)), mini);
	output = getcwd(NULL, 0);
	//if (!output)
	temp = ft_strjoin(output, "/");
	free(output);
	//free(buffer);
	output = ft_strjoin(temp, args[1]);
	free(temp);
	if (chdir(output) == -1)
		printf("minishell: cd: %s: No such file or directory\n", args[1]);
	output = getcwd(NULL, 0);
	//if (!output)
	ft_export_env(ft_strjoin("PWD=", output), mini);
	free(output);
}
