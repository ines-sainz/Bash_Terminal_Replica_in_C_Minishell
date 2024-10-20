/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_retokenize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 12:35:25 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/19 11:29:34 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	ft_count_elements(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

// export a="export a=b"
size_t	ft_retokenize(t_params *params)
{
	t_params	*temp;
	int			n_commands;
	char		**arg_splited;
	int			i;
	t_params	*new_node;
	size_t		args_len;

	//PRINT PARAMS
	// temp = params;
	// while (temp != NULL)
	// {
	// 	printf("DDBB: temp->argc => %i", temp->argc);
	// 	printf("\ttemp->content => %s", temp->content);
	// 	if (temp->type == CMD)
	// 		printf("\ttype: %s", "CMD");
	// 	else if (temp->type == INFILE)
	// 		printf("\ttype: %s", "INFILE");
	// 	else if (temp->type == HERE_DOC)
	// 		printf("\ttype: %s", "HERE_DOC");
	// 	else if (temp->type == OUTFILE)
	// 		printf("\ttype: %s", "OUTFILE");
	// 	else if (temp->type == APPEND)
	// 		printf("\ttype: %s", "APPEND");
	// 	else if (temp->type == PIPE)
	// 		printf("\ttype: %s", "PIPE");
	// 	else if (temp->type == PARAMS)
	// 		printf("\ttype: %s", "PARAMS");
	// 	else if (temp->type == BUILTING)
	// 		printf("\ttype: %s", "BUILTING");
	// 	else if (temp->type == DELIMITER)
	// 		printf("\ttype: %s", "DELIMITER");
	// 	if (temp->quotes == t_true)
	// 		printf("\tquotes: %s\n", "TRUE");
	// 	else if (temp->quotes == t_false)
	// 		printf("\tquotes: %s\n", "FALSE");
	// 	temp = temp->next;
	// }
	//END OF PRINT PARAMS

	temp = params;
	while (temp != NULL)
	{
		n_commands = 1;
		if ((temp->type == BUILTING || temp->type == CMD
				|| temp->type == PARAMS) && temp->quotes == t_false)
		{
			//printf("DDBB: VAMOS A CONTAR ARGUMENTOS!!\n");
			n_commands = ft_count_elements(temp->content, ' ');
		}
		//printf("DDBB: n_commands = %i\n", n_commands);
		if (n_commands > 1)
		{
			arg_splited = ft_split(temp->content, ' ');
			i = 0;
			free(temp->content);
			temp->content = ft_strdup(arg_splited[i]);
			free (arg_splited[i]);
			i++;
			while (i < n_commands)
			{
				//printf("DDBB: ENTRÉ para crear un nodo!!\n");
				new_node = malloc(sizeof(t_params));
				if (!new_node)
					return (-1);
				new_node->content = ft_strdup(arg_splited[i]); // Copia el contenido del argumento
				free (arg_splited[i]);
				//new_node->type = classify_argument(args, argc, heredoc_found); // Clasifica el argumento
				new_node->type = PARAMS;
				new_node->quotes = t_false;
				new_node->next = temp->next;
				temp->next = new_node;
				temp = temp->next;
				i++;
			}
			free (arg_splited);
		}
		temp = temp->next;
	}

	//Recorrer la lista para actualizar argc
	temp = params;
	args_len = 0;
	i = 0;
	while (temp != NULL)
	{
		args_len += ft_strlen(temp->content);
		temp->argc = i;
		i++;
		temp = temp->next;
	}

	//PRINT PARAMS
	// temp = params;
	// while (temp != NULL)
	// {
	// 	printf("DDBB: temp->argc => %i", temp->argc);
	// 	printf("\ttemp->content => %s", temp->content);
	// 	if (temp->type == CMD)
	// 		printf("\ttype: %s", "CMD");
	// 	else if (temp->type == INFILE)
	// 		printf("\ttype: %s", "INFILE");
	// 	else if (temp->type == HERE_DOC)
	// 		printf("\ttype: %s", "HERE_DOC");
	// 	else if (temp->type == OUTFILE)
	// 		printf("\ttype: %s", "OUTFILE");
	// 	else if (temp->type == APPEND)
	// 		printf("\ttype: %s", "APPEND");
	// 	else if (temp->type == PIPE)
	// 		printf("\ttype: %s", "PIPE");
	// 	else if (temp->type == PARAMS)
	// 		printf("\ttype: %s", "PARAMS");
	// 	else if (temp->type == BUILTING)
	// 		printf("\ttype: %s", "BUILTING");
	// 	else if (temp->type == DELIMITER)
	// 		printf("\ttype: %s", "DELIMITER");
	// 	if (temp->quotes == t_true)
	// 		printf("\tquotes: %s\n", "TRUE");
	// 	else if (temp->quotes == t_false)
	// 		printf("\tquotes: %s\n", "FALSE");
	// 	temp = temp->next;
	// }
	//END OF PRINT PARAMS
	return (args_len);
}