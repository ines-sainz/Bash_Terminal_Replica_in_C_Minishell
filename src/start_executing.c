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

void	create_fork(t_execution *iter_exe, t_mini *mini, t_args *args)
{
	pid_t	pid;

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
}

void	close_fds(t_execution *iter_exe)
{
	if (iter_exe->inf_pipe > 0)
		close(iter_exe->inf_pipe);
	if (iter_exe->outf_pipe > 0 && iter_exe->outf_pipe != 1)
		close(iter_exe->outf_pipe);
}

int	start_executing(t_execution *iter_exe, int status,
	t_mini *mini, t_args *args)
{
	while (iter_exe != NULL)
	{
		if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
		{
			close_fds(iter_exe);
			iter_exe = iter_exe->next;
			continue ;
		}
		if (mini->n_commands == 1)
			if (be_built_ins(iter_exe->command) == 1)
				return (check_built_ins(iter_exe->command, iter_exe,
						mini, args));
		create_fork(iter_exe, mini, args);
		close_fds(iter_exe);
		iter_exe = iter_exe->next;
	}
	while (waitpid(-1, &status, 0) != -1)
		continue ;
	// ft_export_env("?=55", mini); Actualizar para Built-ins y execves
	return (WEXITSTATUS(status));
}
