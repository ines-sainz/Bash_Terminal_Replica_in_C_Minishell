/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:20:47 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/13 15:23:29 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cd_arg(char **args, t_mini *mini)
{
	char	*output;
	char	*path;

	if (chdir(args[1]) == -1)
	{
		perror("minishell: cd");
		ft_export_env("?=1", mini);
		return ;
	}
	else
	{
		if (((ft_strcmp(ft_find_env(mini, "OLDPWD"), "/") != 0) && ft_strcmp(args[1], "..") != 0) || ft_strcmp(args[1], "..") == 0)
		{
			output = ft_strjoin("OLDPWD=", ft_get_env("PWD", mini));
			printf("%s\n", output);
			ft_export_env(output, mini);
			free(output);
		}

		if (((ft_strcmp(ft_find_env(mini, "PWD"), "/") != 0) && ft_strcmp(args[1], "..") != 0) || ft_strcmp(args[1], "..") == 0)
		{
			path = getcwd(NULL, 0);
			output = ft_strjoin("PWD=", path);
			free(path);
			printf("%s\n", output);
			ft_export_env(output, mini);
			free(output);
			output = NULL;
			path = NULL;
			//ft_export_env("?=0", mini); // ELIMINAR ??
			return ;
		}
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
		output = NULL;
		path = NULL;
		//ft_export_env("?=0", mini); // ELIMINAR ??
		return ;
	}
}

void	ft_built_cd(char **args, t_mini *mini)
{
	//char	*output;
	//char	*temp;
	int		argc;
	//char	*path;

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
	/* path = ft_strjoin("OLDPWD=", ft_get_env("PWD", mini));
	ft_export_env(path, mini);
	free(path);
	output = getcwd(NULL, 0);
	//if (!output)
	temp = ft_strjoin(output, "/");
	free(output);
	output = ft_strjoin(temp, args[1]);
	free(temp);
	if (chdir(output) == -1)
		printf("minishell: cd: %s: No such file or directory\n", args[1]);
	output = getcwd(NULL, 0);
	//if (!output)
	ft_export_env(ft_strjoin("PWD=", output), mini);
	free(output); */
}
