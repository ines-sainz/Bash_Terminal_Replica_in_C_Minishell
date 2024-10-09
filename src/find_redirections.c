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
	{
		//printf("IN: wrong fd from fill inf\n");
		return ;
	}
	if (iter_exe->inf_pipe != 0)
	{
		//printf("IN: close prev inf fd %i from fill inf\n", iter_exe->inf_pipe);
		close (iter_exe->inf_pipe);
	}
	iter_exe->inf_pipe = open(iter_params->content, O_RDONLY);
	//printf("IN: open fd %i from fill inf\n", iter_exe->inf_pipe);
	if (iter_exe->inf_pipe < 0)
	{
		printf("minishell: %s: No such file or directory\n",
			iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	fill_here_doc(int fd, t_execution *iter_exe, t_mini *mini)
{
	mini->n_here_docs++;
	if (iter_exe->inf_pipe < 0 || iter_exe->outf_pipe < 0)
	{
		//printf("IN: wrong fd, closing here-doc fd fom fill_herre-doc\n");
		close(fd);
		return ;
	}
	if (iter_exe->inf_pipe != 0)
	{
		//printf("IN: close prev inf fd %i from fill here doc\n", iter_exe->inf_pipe);
		close(iter_exe->inf_pipe);
	}
	iter_exe->inf_pipe = fd;
	//printf("IN: set here-doc fd %i as inf fd in fill here-doc\n", fd);
	if (iter_exe->inf_pipe < 0)
	{
		printf("minishell: Error opening here_doc\n");
		ft_export_env("?=1", mini);
	}
}

void	fill_append(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->outf_pipe < 0 || iter_exe->inf_pipe < 0)
	{
		//printf("IN: wrong fd from fill append\n");
		return ;
	}
	if (iter_exe->outf_pipe != 1)
	{
		//printf("IN: close prev outf fd %i from fill append\n", iter_exe->outf_pipe);
		close(iter_exe->outf_pipe);
	}
	iter_exe->outf_pipe = open(iter_params->content, O_WRONLY | O_CREAT
			| O_APPEND, 0777);
	//printf("IN: open fd %i from fill append\n", iter_exe->outf_pipe);
	if (iter_exe->outf_pipe < 0)
	{
		printf("minishell: %s: Permission denied\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}

void	fill_outfile(t_params *iter_params, t_execution *iter_exe, t_mini *mini)
{
	if (iter_exe->outf_pipe < 0 || iter_exe->inf_pipe < 0)
	{
		//printf("IN: wrong fd from fill outf\n");
		return ;
	}
	if (iter_exe->outf_pipe != 1)
	{
		//printf("IN: close prev outf fd %i from fill outf\n", iter_exe->outf_pipe);
		close(iter_exe->outf_pipe);
	}
	iter_exe->outf_pipe = open(iter_params->content, O_RDWR | O_CREAT
			| O_TRUNC, 0777);
	//printf("IN: open fd %i from fill outf\n", iter_exe->outf_pipe);
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
	//printf("IN: open pipe fds. inf: %i outf: %i", pipe_fds[0], pipe_fds[1]);
	if (iter_exe->outf_pipe == 1)
	{
		//printf("IN: put outf pipe %i\n", iter_exe->outf_pipe);
		iter_exe->outf_pipe = pipe_fds[1];
	}
	else if (iter_exe->outf_pipe != 1 && iter_exe->outf_pipe > 0)
	{
		//printf("IN: close outf pipe %i because prev outf %i\n", pipe_fds[1], iter_exe->outf_pipe);
		close(pipe_fds[1]);
	}
	iter_exe = iter_exe->next;
	if (iter_exe->inf_pipe == 0)
	{
		//printf("IN: put inf pipe %i\n", iter_exe->inf_pipe);
		iter_exe->inf_pipe = pipe_fds[0];
	}
	else if (iter_exe->inf_pipe > 0)
	{
		//printf("IN: close inf pipe %i because prev inf %i\n", pipe_fds[0], iter_exe->inf_pipe);
		close(pipe_fds[0]);
	}
}
