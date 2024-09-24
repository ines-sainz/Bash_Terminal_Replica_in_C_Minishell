/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/24 15:18:29 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_tokenize(t_args *args, t_mini *mini)
{
	args->arg = malloc(ft_strlen(args->input) + 1);
	args->arg_ptr = args->arg;
	args->argc = 0;
	args->in_single_quote = t_false;
	args->in_double_quote = t_false;
	if (add_to_args(args, &args->argc, mini) == ERR)
	{
		free (args->arg);
		args->arg = NULL;
		return (ERR);
	}
	free(args->arg);
	args->arg = NULL;
	return (OK);
}

/* int	parse(t_args *args, t_mini *mini)
{
	int		i;
	t_bool	heredoc_found;

	heredoc_found = t_false;
	if (ft_tokenize(args, mini) == ERR)
		return (ERR);
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			add_argument_to_list(args, &i, &heredoc_found);
			free(args->args[i]);
			args->args[i] = NULL;
			args->quotes[i] = t_false;
		}
		i++;
	}
	update_last_command_env_var(args);
	new_red_exe(args, mini);
	close_inf_outf(mini);
	return (OK);
} */

int	parse(t_args *args, t_mini *mini)
{
	int			i;
	t_params	*temp;
	t_bool		heredoc_found;

	heredoc_found = t_false;
	printf("You entered: %s\n", args->input);
	if (ft_tokenize(args, mini) == ERR)
		return (ERR);
	printf("argc ==> %i\n", args->argc);
//	if (ft_built_ins(args, mini) == 1)
//		return (OK);
	i = 0;
	while (i < args->argc)
	{
		if (args->args[i])
		{
			//printf("arg[%d]: %s\n", i, args->args[i]);
			// printf("args->args[%i] ==> %s\n", i, args->args[i]);
			// printf("args->quotes[%i] ==> %i\n", i, args->quotes[i]);
			add_argument_to_list(args, &i, &heredoc_found);
			free(args->args[i]);
			args->args[i] = NULL;
			args->quotes[i] = t_false;

		}
		i++;
	}
	//switch_to_delimiter(args);
	update_last_command_env_var(args);
	new_red_exe(args, mini);
	close_inf_outf(mini);
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
		else if (temp->type == DELIMITER){printf("\ttype: %s", "DELIMITER");}
		if (temp->quotes == t_true){printf("\tquotes: %s\n", "TRUE");}
		else if (temp->quotes == t_false){printf("\tquotes: %s\n", "FALSE");}
		temp = temp->next;
	}
	return (OK);
}
