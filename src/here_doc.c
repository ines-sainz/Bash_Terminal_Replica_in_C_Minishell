/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:19 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/30 14:08:47 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_get_eof(char *eof)
{
	char	*eof_fin;

	eof_fin = (char *)malloc(ft_strlen(eof) + 2);
	ft_strlcpy(eof_fin, eof, ft_strlen(eof) + 1);
	eof_fin[ft_strlen(eof)] = '\n';
	eof_fin[ft_strlen(eof) + 1] = '\0';
	return (eof_fin);
}

/* void signal_here_doc(int sig)
{
	if (sig == SIGINT)
	{
		printf("\nHere-doc interrupted (Ctrl-C)\n");
		rl_replace_line("", 0);  // Limpiar la línea actual
		close(STDIN_FILENO);     // Cerrar stdin para interrumpir el here_doc
	}
} */

/* int	event(void)
{
	return (0);
} */

void	signal_here_doc(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = SIGINT;
		//write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
		//rl_redisplay();
		//rl_done = 1;
	}
}

int	ft_write_temp(int fd, char *eof, char *buffer, t_mini *mini)
{
	t_args	here_doc;
	char	*eof_fin;

	(void)buffer;
	ft_bzero(&here_doc, sizeof(t_args));
	here_doc.mini = mini;
	signal(SIGINT, signal_here_doc);
	g_signal_received = 0;
	while (1)
	{
		//rl_event_hook = event;
		// Verificar si se recibió SIGINT
		if (g_signal_received == SIGINT)
			break ;
		here_doc.arg = readline("> ");
		if (!here_doc.arg)
		{
			ft_dprintf(2, "minishell: warning: here-document delimited by end-of-file (wanted `EOF')\n");
			// Detectar Ctrl-D (EOF)
			break ;
		}
		if (g_signal_received == SIGINT)
		{
			free(here_doc.arg);
			break ;
		}
		eof_fin = ft_get_eof(eof);
		if (!ft_strncmp(eof_fin, here_doc.arg, ft_strlen(here_doc.arg))
			&& ft_strlen(here_doc.arg) > 0)
		{
			free(here_doc.arg);
			free(eof_fin);
			break ;
		}
		expander(&here_doc, mini);
		printf("DB: expanded_arg => %s\n", here_doc.result);
		write(fd, here_doc.result, ft_strlen(here_doc.result));
		write(fd, "\n", 1);
		free(here_doc.arg);
		free(here_doc.result);
		free(eof_fin);
	}
	signal(SIGINT, signal_sigint);
	if (g_signal_received == SIGINT)
	{
		g_signal_received = 0;
		close(fd);
		return (-2);
	}
	return (OK);
}

/* int	ft_write_temp(int fd, char *eof, char *buffer, t_mini *mini)
{
	int		n_bytes;
	t_args	here_doc;

	// Guardar los manejadores de señales originales
	// void (*old_sigint_handler)(int);
	// void (*old_sigquit_handler)(int);

	// Configurar los nuevos manejadores
	// old_sigint_handler = signal(SIGINT, signal_here_doc);//Cambiar SIGINT para el here_doc
	// old_sigquit_handler = signal(SIGQUIT, SIG_IGN);//Ignorar SIGQUIT
	ft_bzero(&here_doc, sizeof(t_args));
	here_doc.mini = mini;
	signal(SIGINT, signal_here_doc);
	g_signal_received = 0;
	while (1)
	{
		// Verificar si se recibió SIGINT
		if (g_signal_received == SIGINT)
			break;
		write(1, ">", 1);
		n_bytes = read(STDIN_FILENO, buffer, 1024);
		if (n_bytes < 0)
		{
			if (errno == EINTR && g_signal_received == SIGINT)
			{
				// Si read() fue interrumpido por SIGINT, salir del bucle
				break;
			}
			else
			{
				// Otro error en read(), salir con error
				perror("read error");
				break;
			}
		}
		// if (n_bytes == 0)
		// 	break ;
		// else if (n_bytes < 0)
		// {
		// 	close(fd);
		// 	return (1);
		// }
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
		// write(fd, expanded_arg, ft_strlen(expanded_arg));
		// free(expanded_arg);
		expander(&here_doc, mini);
		printf("expanded_arg => %s\n", here_doc.result);
		write(fd, here_doc.result, ft_strlen(here_doc.result));
		free(here_doc.result);
	}
	// Restaurar los manejadores de señales originales
	// signal(SIGINT, old_sigint_handler);
	// signal(SIGQUIT, old_sigquit_handler);
	// Si se recibió SIGINT, retorna un error
	signal(SIGINT, signal_sigint);
	if (g_signal_received == SIGINT)
		return (1);
	return (0);
} */

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
		write(fd, buffer, n_bytes);
	}
	return (0);
} */

char	*get_here_doc_file_name(void)
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
		unlink(file_name);
		ft_lstclear(&mini->here_doc_files, free);
		close(fd);
		return (ERR);
	}
	ft_lstadd_back(&mini->here_doc_files, temp_here_doc);
	if (ft_write_temp(fd, param->content, buffer, mini) == -2)
		return (-2);
	close (fd);
	fd = open(file_name, O_RDONLY, 0777);
	return (fd);
}

int	*get_here_doc(t_params *iter_params, t_args *args)
{
	int	i;
	int	*here_doc_fds;

	i = 0;
	here_doc_fds = NULL;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
			i++;
		iter_params = iter_params->next;
	}
	if (i > 0)
		here_doc_fds = malloc(i * sizeof(int *));
	i = 0;
	iter_params = args->params;
	while (iter_params != NULL)
	{
		if (iter_params->type == HERE_DOC)
		{
			here_doc_fds[i] = ft_here_doc(iter_params->next, args->mini, 0);
			i++;
		}
		iter_params = iter_params->next;
	}
	return (here_doc_fds);
}
