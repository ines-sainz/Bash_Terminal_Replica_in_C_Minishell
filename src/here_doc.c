/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:19 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/15 08:21:11 by danjimen         ###   ########.fr       */
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

int	ft_write_temp(int fd, char *eof, char *buffer, t_mini *mini)
{
	int		n_bytes;
	t_args	here_doc;
	//char	*expanded_arg;

	ft_bzero(&here_doc, sizeof(t_args));
	here_doc.mini = mini;
	(void)mini;
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
		//aqui hay que expandir despues de comparar
		here_doc.arg = ft_strdup(buffer);
		//here_doc.args[0] = ft_strdup(buffer);
		//here_doc.args[1] = NULL;
		//here_doc.input = ft_strdup(buffer);
		//here_doc.in_single_quote = t_false;
		// expanded_arg = expander(&here_doc, mini);
		// printf("expanded_arg => %s\n", expanded_arg);
		// write(fd, expanded_arg, ft_strlen(expanded_arg)); //aqui hay que cambiaar n_bytes por el len de la expansion
		// free(expanded_arg);
		expander(&here_doc, mini);
		printf("expanded_arg => %s\n", here_doc.result);
		write(fd, here_doc.result, ft_strlen(here_doc.result)); //aqui hay que cambiaar n_bytes por el len de la expansion
		free(here_doc.result);
	}
	return (0);
}

/* int	ft_write_temp(int fd, char *eof, char *buffer, t_mini *mini)
{
	int		n_bytes;
	t_args	here_doc;
	int		i;

	ft_bzero(&here_doc, sizeof(t_args));
	here_doc.mini = mini;
	(void)mini;
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
		//aqui hay que expandir despues de comparar
		here_doc.input = ft_strdup(buffer);
		if (ft_tokenize(&here_doc, mini) == ERR)
			return (ERR);
		printf("here_doc.argc ==> %i\n", here_doc.argc);
		i = 0;
		while (i < here_doc.argc)
		{
			if (here_doc.args[i])
			{
				printf("arg[%d]: %s\n", i, here_doc.args[i]);
				printf("here_doc.args[%i] ==> %s\n", i, here_doc.args[i]);
				printf("here_doc.quotes[%i] ==> %i\n", i,here_doc.quotes[i]);
				free(here_doc.args[i]);
				here_doc.args[i] = NULL;
				here_doc.quotes[i] = t_false;

			}
			i++;
		}
		write(fd, buffer, n_bytes); //aqui hay que cambiaar n_bytes por el len de la expansion
	}
	return (0);
} */

char	*get_here_doc_file_name()
{
	int		i;
	char	*file_num;
	char	*file_name;

	i = 0;
	file_num = ft_itoa(i);
	file_name = ft_strjoin(file_num, ".tmp");
	free(file_num);
	while (access(file_name, F_OK) == 0)
	{
		i++;
		free(file_name);
		file_num = ft_itoa(i);
		file_name = ft_strjoin(file_num, ".tmp");
		free(file_num);
	}
	return (file_name);
}

int	ft_here_doc(t_params *param, t_mini *mini, int fd)
{
	char	buffer[1024];
	t_list	*temp_here_doc;
	char	*file_name;

	file_name = get_here_doc_file_name();
	fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
		return (free(file_name), ERR);
	temp_here_doc = ft_lstnew(file_name);
	if (temp_here_doc == NULL)
	{
		ft_lstclear(&mini->here_doc_files, free);
		return (ERR);
	}
	ft_lstadd_back(&mini->here_doc_files, temp_here_doc);
	if (ft_write_temp(fd, param->content, buffer, mini) == 1)
		return (ERR);
	close (fd);
	fd = open(file_name, O_RDONLY, 0777);
	return (fd);
}

int	*get_here_doc(t_params *iter_params, t_args *args)
{
	int	i;
	int	*here_doc_fds;
	int	fd;

	i = 0;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
			i++;
		iter_params = iter_params->next;
	}
	here_doc_fds = malloc(i * sizeof(int *));
	i = 0;
	iter_params = args->params;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
		{
			fd = 0;
			here_doc_fds[i] = ft_here_doc(iter_params->next, args->mini, fd);
			i++;
		}
		iter_params = iter_params->next;
	}
	return (here_doc_fds);
}

void	set_here_doc(int fd, t_pipes *iter_pipes, t_mini *mini)
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
