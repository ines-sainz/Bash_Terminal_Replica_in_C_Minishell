/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:19 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/06 19:53:14 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_get_eof(char *eof)
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

int	ft_here_doc(int i, t_params *param)
{
	char	buffer[1024];
	int		fd;
	char	*file_num;
	char	*file_name;

	file_num = ft_itoa(i);
	file_name = ft_strjoin(file_num, ".txt");
	free(file_num);
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (ERR);
	if (ft_write_temp(fd, param->content, buffer) == 1)
		return (ERR);
	close (fd);
	fd = open(file_name, O_RDONLY, 0777);
	free(file_name);
	return (fd);
}

int	*get_here_doc(t_params *iter_params, t_args *args)
{
	int	i;
	int	*here_doc_fds;

	write(1, "a\n", 2);
	i = 0;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
			i++;
		iter_params = iter_params->next;
	}
	here_doc_fds = malloc(i + 1 * sizeof(int *));
	i = 0;
	iter_params = args->params;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
		{
			here_doc_fds[i] = ft_here_doc(i, iter_params->next);
			i++;
		}
		iter_params = iter_params->next;
	}
	return (here_doc_fds);
}

void	set_here_doc(int fd, t_pipes *iter_pipes, t_mini *mini)
{
	//////////// Inés, he añadido esto para poder compilar
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
		//////////// Inés, he comentado esto para poder compilar
		//printf("minishell: Error opening here_doc\n", iter_params->content);
		printf("minishell: Error opening here_doc\n");
		ft_export_env("?=1", mini);
	}
}
