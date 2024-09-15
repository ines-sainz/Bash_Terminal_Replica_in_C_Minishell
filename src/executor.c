/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 09:23:18 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/12 20:40:32 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*int	get_len_matrix(t_params *iter)
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


char	**make_param_matrix(t_params **iter)
{
	char		**param_matrix;
	int			i;
	int			j;

	param_matrix = ft_calloc((get_len_matrix(*iter) + 1), sizeof(char *));
	if (!param_matrix)
		return (0);
	i = 0;
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
	//	if (*iter)
		(*iter) = (*iter)->next;
	}
	return (param_matrix);
}

void	executor(t_args *args)
{
	t_params	*iter;
	char		**param_matrix;
	//pid_t		*num_fork; //INES
	int			i;//no necesario
	//int		status;
	int			num_command;

	num_command = 1;
	//num_fork = malloc(args->mini->n_commands * sizeof(pid_t)); //INES
	iter = args->params;
	while (iter != NULL)
	{
		if (iter->type != PIPE)
		{
			param_matrix = make_param_matrix(&iter);
			printf("The %i commands are:\n", num_command);
			i = 0;
			while (param_matrix[i])
			{
				printf("command = %s\n", param_matrix[i]);
				i++;
			}
			//execute(param_matrix, args, num_command, num_fork);
			num_command++;
		}
		if (iter)
			iter = iter->next;
	}
	//	while (waitpid(-1, &status, 0) != -1)
//		continue ;
//	status = WEXITSTATUS(status);
}*/

/*
cat <hola.txt  > /dev/stdout -e
cat <hola.txt -e > /dev/stdout
cat > hola.txt
cat hola.txt
<<<hola.txt cat			-> hola.txt
cat <hola.txt >			->bash: syntax error near unexpected token `newline'
cat <hola.txt >>		->bash: syntax error near unexpected token `newline'
| cat hola.txt			->bash: syntax error near unexpected token `|'


isainz-r@c1r9s5:~/minishell$ export pipe="|"
isainz-r@c1r9s5:~/minishell$ ls $pipe grep m
ls: cannot access 'grep': No such file or directory
ls: cannot access 'm': No such file or directory
'|'
isainz-r@c1r9s5:~/minishell$ ls -la | grep m
-rwxr-xr-x  1 isainz-r 2019 62440 Sep  2 14:59 minishell
-rw-r--r--  1 isainz-r 2019    33 Sep  2 11:07 README.md
isainz-r@c1r9s5:~/minishell$ ls -la $pipe grep m
ls: cannot access 'grep': No such file or directory
ls: cannot access 'm': No such file or directory
-rw-r--r-- 1 isainz-r 2019 72 Sep  2 14:58 '|'
isainz-r@c1r9s5:~/minishell$ ls > $pipe

isainz-r@c1r9s5:~/minishell$ ls "|" ls
ls: cannot access 'ls': No such file or directory
'|'

isainz-r@c1r13s2:~/minishell$ cat txt.txt | ls > 2 | pwd
/home/isainz-r/minishell

*/