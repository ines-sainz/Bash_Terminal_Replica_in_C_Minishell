/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_red_exe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 10:40:54 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/13 10:40:56 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	syntax_errors(int i, char *redir_content, t_mini *mini)
{
	if (i == 1)
		printf("minishell: syntax error near unexpected token `|'\n");
	else if (i == 2)
		printf("minishell: syntax error near unexpected token `newline'\n");
	else if (i == 3)
		printf("minishell: syntax error near unexpected token `%s'\n",
			redir_content);
	else if (i == 4)
			printf("minishell: syntax error: | at the end of the commands\n");
	ft_export_env("?=2", mini);
	return (ERR);
}

int	errors_and_n_commands(t_params *iter, t_mini *mini)
{
	if (iter->type == '|')
		return(syntax_errors(1, iter->content, mini));
	while (iter != NULL)
	{
		if (iter->type == INFILE || iter->type == OUTFILE
			|| iter->type == HERE_DOC || iter->type == APPEND)
		{
			if (iter->next == NULL)
				return (syntax_errors(2, iter->content, mini));
			if (iter->next->type == PIPE)
				return (syntax_errors(1, iter->content, mini));
			if (iter->next->type == INFILE || iter->next->type == HERE_DOC
				|| iter->next->type == OUTFILE || iter->next->type == APPEND)
				return (syntax_errors(3, iter->next->content, mini));
		}
		if (iter->type == PIPE)
		{
			if (iter->next == NULL)
				return (syntax_errors(4, iter->content, mini));
			mini->n_commands++;
		}
		iter = iter->next;
	}
	printf("the number of commands is: %i\n", mini->n_commands); //control
	return (0);
}

t_execution	*exe_struct_new(int i)
{
	t_execution	*node;

	node = (t_execution *)malloc(sizeof(t_execution));
	if (!node)
		return (NULL);
	node->n_command = i;
	node->command = NULL;
	node->type = PARAMS;
	node->inf_pipe = 0;
	node->outf_pipe = 1;
	node->next = NULL;
	return (node);
}

void	exe_struct_free(t_mini *mini)
{
	t_execution	*next;
	int			i;

	while (mini->exe_command != NULL)
	{
		next = mini->exe_command->next;
		i = 0;
		if (mini->exe_command->command)
		{
			while (mini->exe_command->command[i])
			{
				free(mini->exe_command->command[i]);
				i++;
			}
			free(mini->exe_command->command);
			mini->exe_command->command = NULL;
		}
		free(mini->exe_command);
		mini->exe_command = next;
	}
	mini->exe_command = NULL;
}

void	exe_struct_add_back(t_execution *node, t_mini *mini)
{
	t_execution	*iter;

	if (!mini->exe_command)
	{
		mini->exe_command = node;
		return ;
	}
	iter = mini->exe_command;
	while (iter->next != NULL)
		iter = iter->next;
	iter->next = node;
}

int	create_execution_struct(t_mini *mini)
{
	int			i;
	t_execution	*node;

	i = 1;
	mini->exe_command = NULL;
	while (i <= mini->n_commands)
	{
		node = exe_struct_new(i);
		if (!node)
		{
			exe_struct_free(mini);
			return (ERR);
		}
		exe_struct_add_back(node, mini);
		i++;
	}
	return (0);
}

void	fill_infile(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
		return ;
	if (iter_exe->inf_pipe != 0)
		close (iter_exe->inf_pipe);
	iter_exe->inf_pipe = open(iter_params->content, O_RDONLY);
	if (iter_exe->inf_pipe < 0)
	{
		printf("minishell: %s: No such file or directory\n",
			iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	fill_here_doc(int fd, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
	{
		close(fd);
		return ;
	}
	if (iter_exe->inf_pipe != 0)
		close(iter_exe->inf_pipe);
	iter_exe->inf_pipe = fd;
	if (iter_exe->inf_pipe < 0)
	{
		printf("minishell: Error opening here_doc\n");
		ft_export_env("?=1", mini);
	}
}

void	fill_append(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->outf_pipe < 0 || iter_exe->inf_pipe < 0)
		return ;
	if (iter_exe->outf_pipe != 1)
		close(iter_exe->outf_pipe);
	iter_exe->outf_pipe = open(iter_params->content, O_WRONLY | O_CREAT
		| O_APPEND, 0777);
	if (iter_exe->outf_pipe < 0)
	{
		printf("minishell: %s: Permission denied\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	fill_outfile(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->outf_pipe < 0 || iter_exe->inf_pipe < 0)
		return ;
	if (iter_exe->outf_pipe != 1)
		close(iter_exe->outf_pipe);
	iter_exe->outf_pipe = open(iter_params->content, O_RDWR | O_CREAT
		| O_TRUNC, 0777);
	if (iter_exe->outf_pipe < 0)
	{
		printf("minishell: %s: Permission denied\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	fill_pipe(t_execution *iter_exe)
{
	int	pipe_fds[2];

	pipe(pipe_fds);
	printf("inf %i, outf %i", pipe_fds[0], pipe_fds[1]);
	if (iter_exe->outf_pipe == 1)
		iter_exe->outf_pipe = pipe_fds[1];
	else if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe > 0)
	{
		close(pipe_fds[1]);
		iter_exe->outf_pipe = pipe_fds[1];
	}
	iter_exe = iter_exe->next;
	if (iter_exe->inf_pipe == 0)
		iter_exe->inf_pipe = pipe_fds[0];
	if (iter_exe->inf_pipe > 0)
	{
		close(pipe_fds[0]);
		iter_exe->inf_pipe = pipe_fds[0];
	}
}

void	fill_exe_redirections(t_params *iter_params, t_execution *iter_exe, t_args *args, t_mini *mini)
{
	int			*here_doc_fds;

	here_doc_fds = get_here_doc(iter_params, args);
	iter_params = args->params;
	mini->n_here_docs = 0;
	while (iter_params)
	{
		if (iter_params->type == INFILE)
			fill_infile(iter_params->next, iter_exe, mini);
		else if (iter_params->type == HERE_DOC)
		{
			fill_here_doc(here_doc_fds[mini->n_here_docs], iter_exe, mini);
			mini->n_here_docs++;
		}
		else if (iter_params->type == APPEND)
			fill_append(iter_params->next, iter_exe, mini);
		else if (iter_params->type == OUTFILE)
			fill_outfile(iter_params->next, iter_exe, mini);
		else if (iter_params->type == PIPE)
		{
			fill_pipe(iter_exe);
			iter_exe = iter_exe->next;
		}
		iter_params = iter_params->next;
	}
}

int	red(t_args *args, t_mini *mini)
{
	t_params	*iter_params;
	t_execution	*iter_exe;

	iter_params = args->params;
	mini->n_commands = 1;
	if (errors_and_n_commands(iter_params, mini) == ERR)
		return (ERR);
	if (create_execution_struct(mini) == ERR)
		return (ERR);
	iter_params = args->params;
	iter_exe = mini->exe_command;
	fill_exe_redirections(iter_params, iter_exe, args, mini);
	return (0);
}

/*int	len_matrix(t_params *iter_params)
{
	int			len; //es el número de el comando más todo lo que hay detrás

	len = 0;
	while (iter_params != NULL)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == BUILTING || temp->type == CMD
			|| temp->type == PARAMS)
		{
			n_commands++;
		}
		else if (temp)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	return (n_commands);
}

void	fill_exe_command(t_params *iter_params, t_execution *iter_exe)
{
	int	i;

	while (iter_params != NULL)
	{
		i = 0;
		iter_exe->command = malloc((len_matrix(iter_params) + 1) * sizeof(char *));
		while (iter_params != NULL)
		{
			if (iter_params->type == INFILE || iter_params->type == OUTFILE || iter_params->type == APPEND
				|| iter_params->type == HERE_DOC)
			{
				iter_params = iter_params->next;
				iter_params = iter_params->next;
			}
			if (iter_params->type == PIPE)
				break ;
			if (iter_params)
			{
				iter_exe->type = iter_params->type;
				iter_exe->command[i] = ft_strdup(iter_params->content);
				i++;
				iter_params = iter_params->next;
			}
		}
		iter_exe->command[i] = NULL;
		if (iter_params)
			iter_params = iter_params->next;
	}
}
*/

int	get_len_matrix(t_params *iter)
{
	t_params	*temp;
	int			n_commands; //es el número de el comando más todo lo que hay detrás

	temp = iter;
	n_commands = 0;
	while (temp != NULL)
	{
		if (temp->type == PIPE)
			break ;
		if (temp->type == BUILTING || temp->type == CMD
			|| temp->type == PARAMS)
		{
			n_commands++;
		}
		else if (temp)
			temp = temp->next;
		if (temp)
			temp = temp->next;
	}
	return (n_commands);
}


char	**make_param_matrix(t_params **iter, int i)
{
	char		**param_matrix;
	int			j;

	param_matrix = ft_calloc((get_len_matrix(*iter) + 1), sizeof(char *));
	if (!param_matrix)
		return (0);
	while (*iter != NULL)
	{
		if ((*iter)->type == PIPE)
			break ;
		if ((*iter)->type == BUILTING || (*iter)->type == CMD
			|| (*iter)->type == PARAMS)
		{
			param_matrix[i] = ft_calloc((ft_strlen((*iter)->content) + 1), 1);
			if (!param_matrix[i])
				return (0);
			j = 0;
			while ((*iter)->content[j])
			{
				param_matrix[i][j] = (*iter)->content[j];
				j++;
			}
			i++;
		}
		else
			(*iter) = (*iter)->next;
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
}

void	fill_exe(t_params *iter_params, t_execution *iter_exe)
{
	int	i;

	while (iter_params != NULL)
	{
		if (iter_params->type != PIPE)
		{
			i = 0;
			iter_exe->command = make_param_matrix(&iter_params, i);
			iter_exe = iter_exe->next;
		}
		if (iter_params)
			iter_params = iter_params->next;
	}
}

void	exe(t_args *args, t_mini *mini)
{
	t_execution	*iter_exe;
	t_params	*iter_params;

	iter_exe = mini->exe_command;
	iter_params = args->params;
	fill_exe(iter_params, iter_exe);
	//fill_exe_command(iter_params, iter_exe);
}

void	close_inf_outf(t_mini *mini)
{
	t_execution	*iter;
	t_list		*temp_here_doc;

	iter = mini->exe_command;
	while (iter != NULL)
	{
		if (iter->inf_pipe > 0)
			close(iter->inf_pipe);
		if (iter->outf_pipe > 1)
			close(iter->outf_pipe);
		iter = iter->next;
	}
		temp_here_doc = mini->here_doc_files;
	while (temp_here_doc != NULL)
	{
		unlink(temp_here_doc->content);
		temp_here_doc = temp_here_doc->next;
	}
	ft_lstclear(&mini->here_doc_files, free);
}

int	new_red_exe(t_args *args, t_mini *mini)
{
	t_execution	*iter;
	int			i;

	if (red(args, mini) == ERR)
		return (ERR);
	exe(args, mini);
	iter = mini->exe_command;
	while (iter != NULL)
	{
		printf("n_command: %i   inf: %i      outf: %i   type: %i\n",
			iter->n_command, iter->inf_pipe, iter->outf_pipe, iter->type);
		i = 0;
		while (iter->command[i])
		{
			printf("command = %s\n", iter->command[i]);
			i++;
		}
		iter = iter->next;
	}
	//ejecutar aquí en medio
	return (0);
}