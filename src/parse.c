/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/04 22:26:20 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

#define MAX_ARGS 100

static int	ft_tokenize(t_args *args, int *argc, t_mini *mini)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	*argc = 0;
	args->in_single_quote = t_false;
	args->in_double_quote = t_false;
	if (add_to_args(args, argc, mini) == ERR)
	{
		free (args->arg);
		args->arg = NULL;
		return (ERR);
	}
	free(args->arg);
	args->arg = NULL;
	return (OK);
}

void	close_fds(t_mini *mini)
{
	t_pipes	*pipe_iter;

	pipe_iter = mini->first_pipe;
	while (pipe_iter != NULL)
	{
		if (pipe_iter->inf_pipe > 0)
			close(pipe_iter->inf_pipe);
		if (pipe_iter->outf_pipe > 1)
			close(pipe_iter->outf_pipe);
		pipe_iter = pipe_iter->next;
	}
}

int	parse(t_args *args, t_mini *mini)
{
	int			i;
	t_params	*temp;

	printf("You entered: %s\n", args->input);
	if (ft_tokenize(args, &args->argc, mini) == ERR)
		return (ERR);
	printf("argc ==> %i\n", args->argc);
	if (ft_built_ins(args, mini) == 1)
		return (OK);
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			//printf("arg[%d]: %s\n", i, args->args[i]);
			// printf("args->args[%i] ==> %s\n", i, args->args[i]);
			// printf("args->quotes[%i] ==> %i\n", i, args->quotes[i]);
			add_argument_to_list(args, &i);
			free(args->args[i]);
			args->args[i] = NULL;
			args->quotes[i] = t_false;

		}
		i++;
	}
	update_last_command_env_var(args);
	if (redirector(args, mini) == 1)
		return (ERR);
	executor(args);
	close_fds(mini);
	//unlink("temp.txt"); cerrar los fds 
	printf("Parsed arguments:\n");
	temp = args->params;
	while (temp != NULL)
	{
		//printf("arg[%d]: %s,\t\ttype: %d,\t\tquotes: %i\n", temp->argc, temp->content, temp->type, temp->quotes);
		printf("arg[%d]: %s", temp->argc, temp->content);
		if (temp->type == CMD){printf("\ttype: %s", "CMD");}
		else if (temp->type == INFILE){printf("\ttype: %s", "INFILE");}
		else if (temp->type == HERE_DOC){printf("\ttype: %s", "HERE_DOC");}
		else if (temp->type == OUTFILE){printf("\ttype: %s", "OUTFILE");}
		else if (temp->type == APPEND){printf("\ttype: %s", "APPEND");}
		else if (temp->type == PIPE){printf("\ttype: %s", "PIPE");}
		else if (temp->type == PARAMS){printf("\ttype: %s", "PARAMS");}
		else if (temp->type == BUILTING){printf("\ttype: %s", "BUILTING");}
		if (temp->quotes == t_true){printf("\tquotes: %s\n", "TRUE");}
		else if (temp->quotes == t_false){printf("\tquotes: %s\n", "FALSE");}
		temp = temp->next;
	}
	return (OK);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/