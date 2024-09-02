/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_of_params.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:43:18 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/02 22:47:01 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	del_params(t_args *args)
{
	t_params	*current;
	t_params	*next;

	current = args->params;
	if (args->params != NULL)
	{
		while (current != NULL)
		{
			next = current->next;
			if (current->content != NULL)
				free(current->content);
			free(current);
			current = next;
		}
		args->params = NULL;
	}
}

static t_bool	is_builtin_command(const char *arg)
{
	if (ft_strcmp(arg, "echo") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "cd") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "pwd") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "export") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "unset") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "env") == 0)
		return (t_true);
	else if (ft_strcmp(arg, "exit") == 0)
		return (t_true);
	return (t_false);
}

/* static t_param_type	classify_argument(t_args *args, int *argc)
{
	printf("args->args[%i] ==> %s\n", *argc, args->args[(*argc)]);
	printf("args->quotes[%i] ==> %i\n", *argc, args->quotes[(*argc)]);
	if (ft_strcmp(args->args[*argc], "|") == 0 && args->quotes[*argc] == t_false)
		return (PIPE);
	else if (ft_strcmp(args->args[*argc], "<") == 0  && args->quotes[*argc] == t_false)
		return (INFILE);
	else if (ft_strcmp(args->args[*argc], "<<") == 0  && args->quotes[*argc] == t_false)
		return (HERE_DOC);
	else if (ft_strcmp(args->args[*argc], ">") == 0  && args->quotes[*argc] == t_false)
		return (OUTFILE);
	else if (ft_strcmp(args->args[*argc], ">>") == 0  && args->quotes[*argc] == t_false)
		return (APPEND);
	else if (is_builtin_command(args->args[*argc]) && args->quotes[*argc] == t_false)
		return (BUILTING);
	else if (access(args->args[*argc], X_OK) == 0  && args->quotes[*argc] == t_false)
		return (CMD);
	else
		return (PARAMS); // Se considera un parámetro adicional
} */

static t_param_type	classify_argument(t_args *args, int *argc)
{
	/* printf("args->args[%i] ==> %s\n", *argc, args->args[(*argc)]);
	printf("args->quotes[%i] ==> %i\n", *argc, args->quotes[(*argc)]); */
	if (ft_strcmp(args->args[*argc], "|") == 0)
		return (PIPE);
	else if (ft_strcmp(args->args[*argc], "<") == 0)
		return (INFILE);
	else if (ft_strcmp(args->args[*argc], "<<") == 0)
		return (HERE_DOC);
	else if (ft_strcmp(args->args[*argc], ">") == 0)
		return (OUTFILE);
	else if (ft_strcmp(args->args[*argc], ">>") == 0)
		return (APPEND);
	else if (is_builtin_command(args->args[*argc]))
		return (BUILTING);
	else if (access(args->args[*argc], X_OK) == 0)
		return (CMD);
	else
		return (PARAMS); // Se considera un parámetro adicional
}

t_params *add_argument_to_list(t_args *args, int *argc)
{
	t_params *new_node;
	t_params *temp;

	new_node = malloc(sizeof(t_params));
	if (!new_node)
		return (NULL);
	new_node->content = ft_strdup(args->args[*argc]); // Copia el contenido del argumento
	new_node->argc = *argc; // Asigna el número de argumentos
	new_node->type = classify_argument(args, argc); // Clasifica el argumento
	new_node->next = NULL;

	// Añadir el nuevo nodo al final de la lista
	if (args->params == NULL)
		args->params = new_node;
	else
	{
		temp = args->params;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new_node;
	}
	//printf("node = %s\n", args->params->content);
	return (new_node);
}
