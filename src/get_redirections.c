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

/*void	set_pipe(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{}*/

void	get_redirections(t_args *args, t_mini *mini)
{
	t_params	*iter_params;
	t_pipes		*iter_pipes;
	//int			*here_doc_fds; //////////// Inés, he comentado esto para poder compilar

	//iter_params = args->params;
	//here_doc_fds = get_here_doc(iter_params, args);
	iter_params = args->params;
	iter_pipes = mini->first_pipe;
	while (iter_params != NULL)
	{
		if (iter_params->type == INFILE)
			set_infile(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == HERE_DOC)  /////tiene  que hacerse primero
			set_here_doc(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == APPEND)
			set_append(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == OUTFILE)
			set_outfile(iter_params->next, iter_pipes, mini);
		else if (iter_params->type == PIPE)  ///////por hacerr
		{			
			//set_pipe(iter_params, iter_pipes, mini);
			iter_pipes = iter_pipes->next;
		}
		iter_params = iter_params->next;
	}
}
