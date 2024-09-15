/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_redirections.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 11:33:21 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/06 19:51:02 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*void	set_here_doc(int fd, t_pipes *iter_pipes, t_mini *mini)
{
	//primero abre here_doc luego escribe que se ha roto
	if (iter_pipes->inf_pipe < 0 || iter_pipes->outf_pipe < 0)
	{
		close(fd);
		return ;
	}
	if (iter_pipes->inf_pipe != 0)
		close(iter_pipes->inf_pipe);
	iter_pipes->inf_pipe = fd;
	if (iter_pipes->inf_pipe < 0)
	{
		printf("minishell: Error opening here_doc\n");
		ft_export_env("?=1", mini);
	}
}

void	set_infile(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{
	if (iter_pipes->inf_pipe < 0 || iter_pipes->outf_pipe < 0)
		return ;
	if (iter_pipes->inf_pipe != 0)
		close(iter_pipes->inf_pipe);
	iter_pipes->inf_pipe = open(iter_params->content, O_RDONLY);
	if (iter_pipes->inf_pipe < 0)
	{
		printf("minishell: %s: No such file or directory\n",
			iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	set_outfile(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{
	if (iter_pipes->outf_pipe < 0 || iter_pipes->inf_pipe < 0)
		return ;
	if (iter_pipes->outf_pipe != 1)
		close(iter_pipes->outf_pipe);
	iter_pipes->outf_pipe = open(iter_params->content,  O_RDWR | O_CREAT
			| O_TRUNC, 0777);
	if (iter_pipes->outf_pipe < 0)
	{
		printf("minishell: %s: Permission denied\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	set_append(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{
	if (iter_pipes->outf_pipe < 0 || iter_pipes->inf_pipe < 0)
		return ;
	if (iter_pipes->outf_pipe != 1)
		close(iter_pipes->outf_pipe);
	iter_pipes->outf_pipe = open(iter_params->content, O_WRONLY | O_CREAT
			| O_APPEND, 0777);
	if (iter_pipes->outf_pipe < 0)
	{
		printf("minishell: %s: Permission denied\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	set_pipe(t_pipes *iter_pipes)
{
	int	pipe_fds[2];

	pipe(pipe_fds);
	if (iter_pipes->outf_pipe < 0 || iter_pipes->outf_pipe != 1)
		close(pipe_fds[1]);
	else
		iter_pipes->outf_pipe = pipe_fds[1];
	iter_pipes = iter_pipes->next;
	if (iter_pipes->inf_pipe < 0 || iter_pipes->inf_pipe != 0)
		close(pipe_fds[0]);
	else
		iter_pipes->inf_pipe = pipe_fds[0];
}

void	get_redirections(t_args *args, t_mini *mini)
{
	t_params	*iter_params;
	t_pipes		*iter_pipes;
	int			*here_doc_fds;

	iter_params = args->params;
	here_doc_fds = get_here_doc(iter_params, args);
	iter_params = args->params;
	iter_pipes = mini->first_pipe;
	mini->n_here_docs = 0;
	while (iter_params != NULL)
	{
		if (iter_params->type == INFILE)
			set_infile(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == HERE_DOC)  /////tiene  que hacerse primero
		{
			set_here_doc(here_doc_fds[mini->n_here_docs], iter_pipes, mini);
			mini->n_here_docs++;
		}
		else if (iter_params->type == APPEND)
			set_append(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == OUTFILE)
			set_outfile(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == PIPE)  ///////por hacerr
		{			
			set_pipe(iter_pipes);
			iter_pipes = iter_pipes->next;
		}
		iter_params = iter_params->next;
	}
}*/

/*
pipe redir
*/