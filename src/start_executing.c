/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_executing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 12:13:15 by isainz-r          #+#    #+#             */
/*   Updated: 2024/10/21 13:05:17 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	check_built_ins(char **command, t_execution *iter_exe,
	t_mini *mini, t_args *args)
{
	int		len;
	int		exit_status;

	exit_status = 0;
	dup_redirections(iter_exe);
	close_restant_fds(iter_exe, mini, 1);
	len = ft_strlen(command[0]);
	if (ft_strncmp(command[0], "echo", len) == 0 && len == 4)
		exit_status = ft_built_echo(command);
	else if (ft_strncmp(command[0], "cd", len) == 0 && len == 2)
		exit_status = ft_built_cd(command, mini);
	else if (ft_strncmp(command[0], "pwd", len) == 0 && len == 3)
		exit_status = ft_built_pwd(command, mini);
	else if (ft_strncmp(command[0], "export", len) == 0 && len == 6)
		exit_status = ft_built_export(command, mini);
	else if (ft_strncmp(command[0], "unset", len) == 0 && len == 5)
		exit_status = ft_built_unset(command, mini);
	else if (ft_strncmp(command[0], "env", len) == 0 && len == 3)
		exit_status = ft_built_env(command, mini);
	else if (ft_strncmp(command[0], "exit", len) == 0 && len == 4)
		exit_status = ft_built_exit(args, command);
	return (exit_status);
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

int	create_fork(t_execution *iter_exe, t_mini *mini, t_args *args,
	int exit_status)
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
		free (args->last_history);
		signal(SIGQUIT, SIG_DFL);
		ft_dprintf(2, "DB: (1) exit_status value: %i\n", exit_status);
		if (be_built_ins(iter_exe->command) == 1)
		{
			exit_status = check_built_ins(iter_exe->command,
					iter_exe, mini, args);
			close(mini->standard_fds[0]);
			close(mini->standard_fds[1]);
			ft_dprintf(2, "DB: (2) exit_status value: %i\n", exit_status);
			exe_struct_free(mini);
			exit (exit_status);
		}
		else
		{
			exit_status = execute(iter_exe, mini, args);
			exit (exit_status);
		}
	}
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
		perror("signal");
	return (pid);
}

void	close_fds(t_execution *iter_exe)
{
	if (iter_exe->inf_pipe > 0)
	{
		close(iter_exe->inf_pipe);
	}
	if (iter_exe->outf_pipe > 0 && iter_exe->outf_pipe != 1)
	{
		close(iter_exe->outf_pipe);
	}
}

int	start_executing(t_execution *iter_exe, int status,
	t_mini *mini, t_args *args)
{
	int		exit_status;
	char	*exit_status_itoa;
	char	*exit_status_str;
	pid_t	last_pid;
	pid_t	pid;
	int		last_status;
	t_execution *iter_exe_cpy; // 7 Variables!!!

	exit_status = 0;
	iter_exe_cpy = iter_exe;
	while (iter_exe != NULL)
	{
		if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
		{
			close_fds(iter_exe);
			iter_exe = iter_exe->next;
			continue ;
		}
		if (mini->n_commands == 1)
		{
			if (be_built_ins(iter_exe->command) == 1)
			{
				exit_status = check_built_ins(iter_exe->command, iter_exe,
						mini, args);
				exit_status_itoa = ft_itoa(exit_status);
				exit_status_str = ft_strjoin("?=", exit_status_itoa);
				free(exit_status_itoa);
				ft_export_env(exit_status_str, mini);
				free(exit_status_str);
				// Comprobar si el built-in es exit, y hacer un exit con el valor de return
				ft_dprintf(2, "DB: 1 built-in status value: %i\n", exit_status);
				if (ft_strncmp(iter_exe->command[0], "exit", ft_strlen(iter_exe->command[0])) == 0 && ft_strlen(iter_exe->command[0]) == 4 && exit_status != -1)
				{
					free_at_exit(args);
					printf("exit\n"); // REMOVE BEFORE EVALUATIONS
					exit(exit_status);
				}
				else if ((ft_strncmp(iter_exe->command[0], "exit", ft_strlen(iter_exe->command[0])) == 0 && ft_strlen(iter_exe->command[0]) == 4 && exit_status == -1))
					ft_export_env("?=1", mini);
				return (0);
			}
		}
		last_pid = create_fork(iter_exe, mini, args, 0);
		close_fds(iter_exe);
		iter_exe = iter_exe->next;
	}
	pid = waitpid(-1, &status, 0);
	printf("DB: >> g_signal_received %i\n", g_signal_received);
	while (pid != -1)
	{
		ft_dprintf(2, "DB: PID %i finish whit status: %i\n", pid, WEXITSTATUS(status));
		if (pid == last_pid)
		{
			last_status = WEXITSTATUS(status);
			//last_status = WIFSIGNALED(status) + 128; // Revisar para señales
		}
		pid = waitpid(-1, &status, 0);
	}
	exit_status_itoa = ft_itoa(last_status);
	exit_status_str = ft_strjoin("?=", exit_status_itoa);
	free(exit_status_itoa);
	ft_export_env(exit_status_str, mini);
	free(exit_status_str);
	while (iter_exe_cpy != NULL)
	{
		ft_dprintf(2, "DB: iter_exe->command[0] = %s\n", iter_exe_cpy->command[0]);
		ft_dprintf(2, "DB: ft_strlen(iter_exe_cpy->command[0]) = %i\n", ft_strlen(iter_exe_cpy->command[0]));
		ft_dprintf(2, "DB: last_status = %i\n", last_status);
		ft_dprintf(2, "DB: status = %i\n", status);
		ft_dprintf(2, "DB: WEXITSTATUS(status) = %i\n", WEXITSTATUS(status));
		// Caso especial en el que exit devuelve -1, pero debe actualizar $? a 1
		//if ((ft_strncmp(iter_exe_cpy->command[0], "exit", ft_strlen(iter_exe_cpy->command[0])) == 0 && ft_strlen(iter_exe_cpy->command[0]) == 4 && last_status == -1))
		if (ft_strncmp(iter_exe_cpy->command[0], "exit", ft_strlen(iter_exe_cpy->command[0])) == 0 && ft_strlen(iter_exe_cpy->command[0]) == 4 && last_status == 255 && iter_exe_cpy->command[2] != NULL)
			ft_export_env("?=1", mini);
		iter_exe_cpy = iter_exe_cpy->next;
	}
	//ft_export_env("?=55", mini); //Actualizar para Built-ins y execves
	return (last_status);
}
