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

void	free_last_env(t_mini *mini)
{
	int	i;

	i = 0;
	if (mini->env)
	{
		while (mini->env[i])
		{
			free(mini->env[i]);
			i++;
		}
		free(mini->env);
		mini->env = NULL;
	}
}

void	create_env_matrix(t_mini *mini)
{
	char	*temp;
	char	*var_temp;
	char	*env_one_line;

	free_last_env(mini);
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

void	dup_redirections(t_execution *iter_exe)
{
	if (iter_exe->inf_pipe != 0)
	{
		dup2(iter_exe->inf_pipe, 0);
		close(iter_exe->inf_pipe);
	}
	if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe != 2)
	{
		dup2(iter_exe->outf_pipe, 1);
		close(iter_exe->outf_pipe);
	}
}

int	execute(t_execution *iter_exe, t_mini *mini)
{
	char	*path_mid;
	char	*path_command;

	create_env_matrix(mini);
	dup_redirections(iter_exe);
	path_mid = NULL;
	path_command = get_path_command(iter_exe->command, mini->env, path_mid);
	if (path_command && access(path_command, X_OK) == 0)
		execve(path_command, iter_exe->command, mini->env);
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
	//returns y frees
	return (127);
}

int	check_built_ins(char **command, t_execution *iter_exe,
	t_mini *mini, t_args *args)
{
	int	len;

	dup_redirections(iter_exe);
	len = ft_strlen(command[0]);
	if (ft_strncmp(command[0], "echo", len) == 0 && len == 4)
		ft_built_echo(command);
	else if (ft_strncmp(command[0], "cd", len) == 0 && len == 2)
		ft_built_cd(command, mini);
	else if (ft_strncmp(command[0], "pwd", len) == 0 && len == 3)
		ft_built_pwd(command, mini);
	else if (ft_strncmp(command[0], "export", len) == 0 && len == 6)
		ft_built_export(command, mini);
	else if (ft_strncmp(command[0], "unset", len) == 0 && len == 5)
		ft_built_unset(command, mini);
	else if (ft_strncmp(command[0], "env", len) == 0 && len == 3)
		ft_print_env(mini);
	else if (ft_strncmp(command[0], "exit", len) == 0 && len == 4)
		ft_built_exit(args, command, mini);
	return (0);
}

int	be_built_ins(char **command)
{
	int	len;

	len = ft_strlen(command[0]);
	if (ft_strncmp(command[0], "echo", len) == 0 && len == 4)
		return (1);
	else if (ft_strncmp(command[0], "cd", len) == 0 && len == 2)
		return (1);
	else if (ft_strncmp(command[0], "pwd", len) == 0 && len == 3)
		return (1);
	else if (ft_strncmp(command[0], "export", len) == 0 && len == 6)
		return (1);
	else if (ft_strncmp(command[0], "unset", len) == 0 && len == 5)
		return (1);
	else if (ft_strncmp(command[0], "env", len) == 0 && len == 3)
		return (1);
	else if (ft_strncmp(command[0], "exit", len) == 0 && len == 4)
		return (1);
	return (0);
}

t_execution	*close_redirections(t_execution *iter_exe)
{
	if (iter_exe->inf_pipe > 0)
		close(iter_exe->inf_pipe);
	if (iter_exe->outf_pipe > 0 && iter_exe->outf_pipe != 1 && iter_exe->outf_pipe != 2)
		close(iter_exe->outf_pipe);
	return (iter_exe->next);
}



int	start_executing(t_execution *iter_exe, t_mini *mini, t_args *args)
{
	pid_t	pid;
	int		status;

	while (iter_exe != NULL)
	{
		if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
		{
			iter_exe = close_redirections(iter_exe);
			continue ;
		}
		if (mini->n_commands == 1)
			if (be_built_ins(iter_exe->command) == 1)
				return (check_built_ins(iter_exe->command, iter_exe, mini, args));
		pid = fork();
		if (pid == -1)
		{
			perror("fork failed");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (be_built_ins(iter_exe->command) == 1)
			{
				check_built_ins(iter_exe->command, iter_exe, mini, args);
				exit (0);
			}
			else
				execute(iter_exe, mini);
		}
		iter_exe = close_redirections(iter_exe);
	}
	while (waitpid(-1, &status, 0) != -1)
			continue ;
	// ft_export_env("?=55", mini); Actualizar para Built-ins y execves
	return (WEXITSTATUS(status));
}
