/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:20:47 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/13 19:56:18 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cd_arg(char **args, t_mini *mini)
{
	char	*output;
	char	*path;
	char	*buffer;

	if (chdir(args[1]) == -1)
	{
		perror("minishell: cd");
		ft_export_env("?=1", mini);
		return ;
	}
	else
	{
		buffer = getcwd(NULL, 0);
		if (ft_strcmp(ft_find_env(mini, "OLDPWD"), buffer) != 0)
		{
			output = ft_strjoin("OLDPWD=", ft_get_env("PWD", mini));
			printf("%s\n", output);
			ft_export_env(output, mini);
			free(output);
		}
		free(buffer);
		buffer = getcwd(NULL, 0);
		if (ft_strcmp(ft_find_env(mini, "PWD"), buffer) != 0)
		{
			path = getcwd(NULL, 0);
			output = ft_strjoin("PWD=", path);
			free(path);
			printf("%s\n", output);
			ft_export_env(output, mini);
			free(output);
			free(buffer);
			//output = NULL;
			//path = NULL;
			//ft_export_env("?=0", mini); // ELIMINAR ??
			return ;
		}
		free(buffer);
	}
}

static void	cd_home(t_mini *mini)
{
	char	*output;
	char	*path;

	if (chdir(ft_find_env(mini, "HOME")) == -1)
	{
		perror("minishell: cd");
		ft_export_env("?=1", mini);
		return ;
	}
	else
	{
		output = ft_strjoin("OLDPWD=", ft_get_env("PWD", mini));
		printf("OLDPWD => %s\n", output);
		ft_export_env(output, mini);
		free(output);
		path = ft_find_env(mini, "HOME");
		output = ft_strjoin("PWD=", path);
		free(path);
		printf("PWD => %s\n", output);
		ft_export_env(output, mini);
		free(output);
		//output = NULL;
		//path = NULL;
		//ft_export_env("?=0", mini); // ELIMINAR ??
		return ;
	}
}

void	ft_built_cd(char **args, t_mini *mini)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		printf("minishell: cd: too many arguments\n");
		ft_export_env("?=1", mini);
		return ;
	}
	else if (argc == 1)
		cd_home(mini);
	else if (argc == 2)
		cd_arg(args, mini);
}
