/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_executing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:13:15 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/21 19:52:45 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*check_command(char **path_list, char *path_mid, char *kid)
{
	int		j;
	int		i;
	char	*path_fin;

	j = -1;
	while (path_list[++j])
	{
		path_mid = ft_strjoin(path_list[j], "/");
		path_fin = ft_strjoin(path_mid, kid);
		free(path_mid);
		if (access(path_fin, X_OK) == 0)
		{
			i = 0;
			while (path_list[i])
			{
				free(path_list[i]);
				i++;
			}
			free(path_list);
			return (path_fin);
		}
		free(path_fin);
	}
	return (NULL);
}

char	*get_path_command(char **kid, char **env, char *path_mid)
{
	int		i;
	char	*path;
	char	**path_list;

	if (ft_strchr(kid[0], '/'))
		if (access(kid[0], X_OK) == 0)
			return (ft_strdup(kid[0]));
	i = -1;
	while (env[++i])
	{
		if (!ft_strncmp("PATH=", env[i], 5))
		{
			path_list = ft_split(env[i], ':');
			path = check_command(path_list, path_mid, kid[0]);
			if (path)
				return (path);
		}
	}
	i = 0;
	while (path_list[i])
	{
		free(path_list[i]);
		i++;
	}
	return (free(path_list), NULL);
}

void	create_env_matrix(t_mini *mini)
{
	char	*temp;
	char	*var_temp;
	char	*env_one_line;
	int		i;

	i = 0;
	if (mini->env)
	{
		while (mini->env[i])
		{
			free(mini->env[i]);
			i++;
		}
		free(mini->env);
	}
	env_one_line = NULL;
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter != NULL)
	{
		temp = ft_strjoin(mini->env_iter->variable, "=");
		var_temp = ft_strjoin(temp, mini->env_iter->content);
		free(temp);
		temp = ft_strjoin(var_temp, "\n");
		free(var_temp);
		var_temp = ft_strjoin(env_one_line, temp);
		free(temp);
		env_one_line = var_temp;
		mini->env_iter = mini->env_iter->next;
	}
	mini->env = ft_split(env_one_line, '\n');
	free(env_one_line);
}

int	execute(t_execution *iter_exe, t_mini *mini)
{
	char	*path_mid;
	char	*path_command;
	//int		i;

	create_env_matrix(mini);
	if (iter_exe->inf_pipe != 0)
	{
		dup2(iter_exe->inf_pipe, 0);
		close(iter_exe->inf_pipe);
	}
	if (iter_exe->outf_pipe != 1)
	{
		dup2(iter_exe->outf_pipe, 1);
		close(iter_exe->outf_pipe);
	}
	path_mid = NULL;
	path_command = get_path_command(iter_exe->command, mini->env, path_mid);
	if (path_command && access(path_command, X_OK) == 0)
	{
		printf("execve-> path_command: %s\n", path_command);
		execve(path_command, iter_exe->command, mini->env);
	}
	else
	{
		if (access(path_command, X_OK) != 0)
		{
			write(2, "minishell: Command: Permission denied\n", 39);
			exit (127);
		}
		else
		{
			printf("minishell: Command: Not a directory\n");
			exit (127);
		}
	}
	/* i = 0;
	while (iter_exe->command[i])
	{
		printf("execve com: %s\n", iter_exe->command[i]);
		i++;
	} */
	//i = 0;
	//returns y frees
	return (127);
}

int	check_built_ins(char **command, t_mini *mini, t_args *args)
{
	int	len;

	len = ft_strlen(command[0]);
	if (ft_strncmp(command[0], "echo", len) == 0 && len == 4)
	{
		ft_built_echo(command);
		return (1);
	}
	if (ft_strncmp(command[0], "cd", len) == 0 && len == 2)
	{
		ft_built_cd(command, mini);
		return (1);
	}
	if (ft_strncmp(command[0], "pwd", len) == 0 && len == 3)
	{
		ft_built_pwd(command, mini);
		return (1);
	}
	if (ft_strncmp(command[0], "export", len) == 0 && len == 6)
	{
		ft_built_export(command, mini);
		return(1);
	}
	if (ft_strncmp(command[0], "unset", len) == 0 && len == 5)
	{
		ft_built_unset(command, mini);
		return (1);
	}
	if (ft_strncmp(command[0], "env", len) == 0 && len == 3)
	{
		ft_print_env(mini);
		return (1);
	}
	if (ft_strncmp(command[0], "exit", len) == 0 && len == 4)
	{
		ft_built_exit(args, command, mini);
		return (1);
	}
	return (0);
}

int	start_executing(t_execution *iter_exe, t_mini *mini, t_args *args)
{
	pid_t	pid;
	int		status;
	pid_t	pids[mini->n_commands]; // Array para almacenar los PID de cada proceso
	int		i;

	i = 0;
	while (iter_exe != NULL)
	{
		if (mini->n_commands == 1)
		{
			if (check_built_ins(iter_exe->command, mini, args) == 1)
				return (0);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		printf("pid: %i\n", pid);
		pids[i++] = pid; // Guardar el PID en el array
		if (pid == 0)
		{
			/* if (iter_exe->inf_pipe != 0)
				close(iter_exe->inf_pipe);
			if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe != 2)
				close(iter_exe->outf_pipe); */
			if (check_built_ins(iter_exe->command, mini, args) == 0)
				execute(iter_exe, mini);
			else
				exit (0);
		}
		if (iter_exe->inf_pipe != 0)
			close(iter_exe->inf_pipe);
		if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe != 2)
			close(iter_exe->outf_pipe);
		iter_exe = iter_exe->next;
	}
	i = 0;
	while(i < mini->n_commands)
	{
		if (waitpid(pids[i], &status, 0) == -1)
			perror("waitpid failed");
		i++;
	}
	/* while (waitpid(-1, &status, 0) != -1)
		continue ; */
	// ft_export_env("?=55", mini); Actualizar para Built-ins y execves
	return (WEXITSTATUS(status));
}
