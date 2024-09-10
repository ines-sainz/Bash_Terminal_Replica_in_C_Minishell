/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/10 11:11:26 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_list(t_mini *mini)
{
	int	i;

	i = 1;
	while (i <= mini->nbr_env_nodes)
	{
		mini->env_iter = mini->env_first_node;
		while (mini->env_iter != NULL )
		{
			if (mini->env_iter->order == i)
			{
				if (ft_strlen(mini->env_iter->variable) == 1
					&& (mini->env_iter->variable[0] == '_'
						|| mini->env_iter->variable[0] == '?'))
					break ;
				else if (mini->env_iter->content)
					printf("declare -x %s=\"%s\"\n", mini->env_iter->variable,
						mini->env_iter->content);
				else
					printf("declare -x %s\n", mini->env_iter->variable);
				break ;
			}
			mini->env_iter = mini->env_iter->next;
		}
		i++;
	}
}

static void	create_nodes_order(t_mini *mini)
{
	t_env	*next_node;
	int		less_quantity;

	mini->env_iter = mini->env_first_node;
	while (mini->env_iter)
	{
		less_quantity = 0;
		next_node = mini->env_first_node;
		while (next_node)
		{
			if (ft_strcmp(mini->env_iter->variable, next_node->variable) > 0)
				less_quantity++;
			next_node = next_node->next;
		}
		mini->env_iter->order = less_quantity + 1;
		mini->env_iter = mini->env_iter->next;
	}
}

static void	count_nodes(t_mini *mini)
{
	mini->nbr_env_nodes = 0;
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter != NULL)
	{
		mini->nbr_env_nodes++;
		mini->env_iter->order = 0;
		mini->env_iter = mini->env_iter->next;
	}
}

void	ft_built_export(char **args, t_mini *mini)
{
	int	argc;
	int	flag;

	argc = 0;
	flag = 0;
	while (args[argc])
	{
		if (args[argc][0] != '\0')
			flag++;
		argc++;
	}
	if (argc == 1 || flag == 1)
	{
		count_nodes(mini);
		printf("Number of nodes: %i\n", mini->nbr_env_nodes); // ELIMINAR
		create_nodes_order(mini);
		print_list(mini);
		ft_export_env("?=0", mini);
	}
	else
		export_args(args, mini);
}
