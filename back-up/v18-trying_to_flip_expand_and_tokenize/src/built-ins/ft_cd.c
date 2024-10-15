/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:20:47 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/03 11:36:35 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_pwd(char *buffer, char *output, char	*path, t_mini *mini)
{
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
		ft_export_env("?=0", mini);
		return ;
	}
	free(buffer);
}

static void	update_oldpwd(char *buffer, char *output, t_mini *mini)
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
}

static int	cd_arg(char **args, t_mini *mini)
{
	char	*buffer;
	char	*output;
	char	*path;

	buffer = NULL;
	output = NULL;
	path = NULL;
	if (chdir(args[1]) == -1)
	{
		perror("minishell: cd");
		//ft_export_env("?=1", mini);
		return (1);
	}
	else
	{
		update_oldpwd(buffer, output, mini);
		update_pwd(buffer, output, path, mini);
	}
	return (0);
}

static int	cd_home(t_mini *mini)
{
	char	*output;
	char	*path;
	char	*home;

	home = ft_find_env(mini, "HOME");
	if (home != NULL && chdir(home) == -1)
	{
		perror("minishell: cd");
		//ft_export_env("?=1", mini);
		return (1);
	}
	else
	{
		output = ft_strjoin("OLDPWD=", ft_get_env("PWD", mini));
		printf("DB: OLDPWD => %s\n", output);
		ft_export_env(output, mini);
		free(output);
		path = ft_strdup(ft_find_env(mini, "HOME"));
		output = ft_strjoin("PWD=", path);
		free(path);
		path = NULL;
		printf("DB: PWD => %s\n", output);
		ft_export_env(output, mini);
		free(output);
		//ft_export_env("?=0", mini);
		return (0);
	}
}

int	ft_built_cd(char **args, t_mini *mini)
{
	int	argc;
	int	exit_status;

	argc = 0;
	exit_status = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		printf("minishell: cd: too many arguments\n");
		//ft_export_env("?=1", mini);
		return (1);
	}
	else if (argc == 1)
		exit_status = cd_home(mini);
	else if (argc == 2)
		exit_status = cd_arg(args, mini);
	return (exit_status);
}
