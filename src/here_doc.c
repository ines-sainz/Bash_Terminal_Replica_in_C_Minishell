/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:19 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/05 12:25:21 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*char	*ft_get_eof(char *eof)
{
	char	*eof_fin;

	eof_fin = (char *)malloc(ft_strlen(eof + 2));
	ft_strlcpy(eof_fin, eof, ft_strlen(eof) + 1);
	eof_fin[ft_strlen(eof)] = '\n';
	eof_fin[ft_strlen(eof) + 1] = '\0';
	return (eof_fin);
}

int	ft_write_temp(int fd, char *eof, char *buffer)
{
	int	n_bytes;

	while (1)
	{
		write(1, ">", 1);
		n_bytes = read(STDIN_FILENO, buffer, 1024);
		if (n_bytes == 0)
			break ;
		else if (n_bytes < 0)
		{
			close(fd);
			return (1);
		}
		buffer[n_bytes] = '\0';
		if (!ft_strncmp(ft_get_eof(eof), buffer, n_bytes))
			break ;
		write(fd, buffer, n_bytes);
	}
	return (0);
}

int	ft_here_doc(int i, int * here_doc_fds, t_params *param, t_args *args)
{
	char	buffer[1024];
	int		status;
	char	*file_num;
	char	*file_name;

	file_num = ft_itoa(i);
	file_name = ft_strjoin(file_num, ".txt");
	free(file_num);
	here_doc_fds[i] = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (here_doc_fds[i] < 0)
		return (ERR);
	if (ft_write_temp(here_doc_fds[i], param->content, buffer) == 1)
		return (ERR);
	close (here_doc_fds[i]);
	here_doc_fds[i] = open(file_name, O_RDONLY, 0777);
	free(file_name);
}

int	*get_here_doc(t_params *iter_params, t_args *args, t_mini *mini)
{
	int	i;
	int	*here_doc_fds;

	i = 0;
	while (iter_params != NULL)
		if (iter_params->type == HERE_DOC)
			i++;
	here_doc_fds = malloc(i + 1 * sizeof(int *));
	i = 0;
	iter_params = args->params;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
		{
			ft_here_doc(i, here_doc_fds, iter_params->next, args);
			i++;
		}
		iter_params = iter_params->next;
	}
}*/

void	set_here_doc(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{
	//primero abre here_doc luego escribe que se ha roto
	if (iter_pipes->inf_pipe < 0 || iter_pipes->outf_pipe < 0)
		return ;
	if (iter_pipes->inf_pipe != 0)
		close(iter_pipes->inf_pipe);
	iter_pipes->inf_pipe = open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (iter_pipes->inf_pipe < 0)
	{
		printf("minishell: Error opening here_doc\n", iter_params->content);
		ft_export_env("?=1", mini);
	}
}
