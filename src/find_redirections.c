/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 10:24:53 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/16 10:24:55 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	fill_infile(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
		return ;
	if (iter_exe->inf_pipe != 0)
	{
		//printf("DB: 1 inf close: %i\n", iter_exe->inf_pipe);//
		close (iter_exe->inf_pipe);
	}
	iter_exe->inf_pipe = open(iter_params->content, O_RDONLY);
	//printf("DB: 2 inf open: %i\n", iter_exe->inf_pipe);//
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
		//printf("DB: 3 here_doc close: %i\n", fd);//
		close(fd);
		return ;
	}
	if (iter_exe->inf_pipe != 0)
	{
		//printf("DB: 4 inf close: %i\n", iter_exe->inf_pipe);//
		close(iter_exe->inf_pipe);
	}
	iter_exe->inf_pipe = fd;
	//printf("DB: 5 here_doc set: %i\n", fd);//
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
	{
		//printf("DB: 6 outf close: %i\n", iter_exe->outf_pipe);//
		close(iter_exe->outf_pipe);
	}
	iter_exe->outf_pipe = open(iter_params->content, O_WRONLY | O_CREAT
		| O_APPEND, 0777);
	//printf("DB: 7 append open: %i\n", iter_exe->outf_pipe);//
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
	{
		//printf("DB: 8 outf close: %i\n", iter_exe->outf_pipe);
		close(iter_exe->outf_pipe);
	}
	iter_exe->outf_pipe = open(iter_params->content, O_RDWR | O_CREAT
		| O_TRUNC, 0777);
	//printf("DB: 9 outf open: %i\n", iter_exe->outf_pipe);
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
	//printf("DB: 10 inf %i, outf %i\n", pipe_fds[0], pipe_fds[1]);
	if (iter_exe->outf_pipe == 1)
	{
		//printf("DB: 11 pipe set outf: %i\n", pipe_fds[1]);
		iter_exe->outf_pipe = pipe_fds[1];
	}
	else if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe > 0)
	{
		//printf("DB: 12 pipe outf close: %i\n", pipe_fds[1]);
		close(pipe_fds[1]);
		//printf("DB: 13 out_still: %i\n", iter_exe->outf_pipe);
	}
	iter_exe = iter_exe->next;
	if (iter_exe->inf_pipe == 0)
	{
		iter_exe->inf_pipe = pipe_fds[0];
		//printf("DB: 14 pipe set inf: %i\n", iter_exe->inf_pipe);
	}
	else if (iter_exe->inf_pipe > 0)
	{
		//printf("DB: 15 inf close: %i\n", pipe_fds[0]);
		close(pipe_fds[0]);
		//printf("DB: 16 inf_still: %i\n", iter_exe->inf_pipe);
	}
}
