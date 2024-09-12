/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 12:44:27 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/12 12:44:28 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*int	built(char *command)
{
	int	len;

	len = ft_strlen(command);
	if (ft_strncmp(command, "echo", len) == 0 && len == 4)
		return (0);
	else if (ft_strncmp(command, "cd", len) == 0 && len == 2)
		return (0);
	else if (ft_strncmp(command, "pwd", len) == 0 && len == 3)
		return (0);
	else if (ft_strncmp(command, "export", len) == 0 && len == 6)
		return (0);
	else if (ft_strncmp(command, "unset", len) == 0 && len == 5)
		return (0);
	else if (ft_strncmp(command, "env", len) == 0 && len == 3)
		return (0);
	else if (ft_strncmp(command, "exit", len) == 0 && len == 4)
		return (0);
	return (1);
}

char	**env_to_matrix(t_mini *mini)
{
	char	**env_matrix;
	char	*variable;
	char	*content;
	char	*temp;
	int		i;

	mini->env_iter = mini->env_first_node;
	env_matrix = malloc((mini->nbr_env_nodes + 1) * sizeof(char *));
	i = 0;
	while (mini->env_iter != NULL)
	{
		variable = ft_strdup(mini->env_iter->variable);
		content = ft_strdup(mini->env_iter->content);
		temp = ft_strjoin(variable, "=");
		free(variable);
		env_matrix[i] = ft_strjoin(temp, content);
		free(content);
		i++;
		mini->env_iter = mini->env_iter->next;
	}
	env_matrix[mini->nbr_env_nodes] = NULL;
	return (env_matrix);
}

char	*ft_check_command(char **path_list, char *path_mid, char *kid)
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
			path = ft_check_command(path_list, path_mid, kid[0]);
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

void	execute_normal_commands(char **command, int num_command, t_mini *mini)
{
	char	*path_command;
	char	*path_mid;
	t_pipes	*iter_pipes;
	char	**env_matrix;

	iter_pipes = mini->first_pipe;
	while (iter_pipes != NULL && iter_pipes->num_command != num_command)
		iter_pipes = iter_pipes->next;
	dup2(iter_pipes->inf_pipe, 0);
	dup2(iter_pipes->outf_pipe, 1);
	close(iter_pipes->inf_pipe);
	close(iter_pipes->outf_pipe);
	path_mid = NULL;
	env_matrix = env_to_matrix(mini);
	path_command = get_path_command(command, env_matrix, path_mid);
	if (path_command)
		execve(path_command, command, env_matrix);
	//free if !path_command
}

void	execute(char **command, t_args *args, int num_command, pid_t *num_fork)
{
	//if (mini->n_commands == 1 && args->params->quotes == FALSE)
	//{}
	if (args->mini->n_commands == 1 && built(command[0]) == 0) //args->params->type == BUILTING)
	{
		ft_built_ins(args, args->mini);
		return ;
	}
	else
	{
		num_fork[num_command] = fork();
		if (num_fork == 0)
		{
			//if (built(command[0]) == 0)
			//{
				//ft_built_ins();
			//}
			//else
			//{}
			execute_normal_commands(command, num_command, args->mini);
		}
	}
}
*/