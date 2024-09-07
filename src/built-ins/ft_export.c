/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/07 21:32:50 by danjimen         ###   ########.fr       */
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
		while (mini->env_iter != NULL)
		{
			if (mini->env_iter->order == i)
			{
				//printf("%s order = %i\n", mini->env_iter->variable, mini->env_iter->order);
				if (mini->env_iter->content)
					printf("declare -x %s=\"%s\"\n", mini->env_iter->variable,
						mini->env_iter->content);
				else
					printf("declare -x %s=\n", mini->env_iter->variable);
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

void	ft_built_export(t_args *args, t_mini *mini)
{
	int	i;
	int	argc;

	argc = 0;
	while (args->args[argc])
		argc++;
	if (argc == 1)
	{
		count_nodes(mini);
		printf("Number of nodes: %i\n", mini->nbr_env_nodes);
		create_nodes_order(mini);
		//printf(">>VAMOS A IMPRIMIR LA LISTA<<\n");
		print_list(mini);
		return ;
	}
	i = 1;
	while (args->args[i])
	{
		//if (ft_strchr(args->args[i], '='))
		ft_export_env(args->args[i], mini);
		i++;
	}
}
