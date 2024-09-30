/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:50:09 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/25 12:50:11 by isainz-r         ###   ########.fr       */
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
		if (env_one_line)
			free(env_one_line);
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

void	close_restant_fds(t_execution *exe_command, t_mini *mini)
{
	t_execution *iter_exe;

	close(mini->standard_fds[0]);
	close(mini->standard_fds[1]);
	iter_exe = exe_command;
	while (iter_exe != NULL)
	{
		if (iter_exe->n_command != exe_command->n_command)
			close_fds(iter_exe);
		iter_exe = iter_exe->next;
	}
}

void	free_and_close_all(t_mini *mini, t_args *args, char *path_command, t_execution *exe_comamnd)
{
	t_execution	*iter_exe;
	int			i;

	if (path_command)
		free(path_command);
	free(mini->user_prompt);
	iter_exe = mini->exe_command;
	while (iter_exe != NULL)
	{
		i = 0;
		if (iter_exe->n_command != exe_comamnd->n_command)
		{
			while (iter_exe->command[i])
				free(iter_exe->command[i++]);
			free(iter_exe->command);
		}
		iter_exe = iter_exe->next;
	}
	ft_lstclear(&mini->here_doc_files, free);
	free_last_env(mini);
	free_env(mini);
	(void)args;
}

int	execute(t_execution *iter_exe, t_mini *mini, t_args *args)
{
	char	*path_mid;
	char	*path_command;

	create_env_matrix(mini);
	dup_redirections(iter_exe);
	path_mid = NULL;
	path_command = get_path_command(iter_exe->command, mini->env, path_mid);
	if (path_command)
	{
		if (access(path_command, X_OK) == 0)
		{
			close_restant_fds(mini->exe_command, mini);
			execve(path_command, iter_exe->command, mini->env);
		}
		else
			write(2, "minishell: Command: Permission denied\n", 39);
		free(path_command);
	}
	else
	{
		free_and_close_all(mini, args, path_command, iter_exe);
		close(mini->standard_fds[0]);
		close(mini->standard_fds[1]);
		printf("minishell: Command: Not a directory\n");
	}
	exit(127);
	//returns y frees
	return (127);
}
