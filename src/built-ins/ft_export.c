/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/07 19:03:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/09/07 19:05:09 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_built_export(t_args *args, t_mini *mini)
{
	int	i;

	if (args->argc == 1)
	{
		mini->env_iter = mini->env_first_node;
		while (mini->env_iter != NULL)
		{
			printf("declare -x %s=\"%s\"\n", mini->env_iter->variable,
				mini->env_iter->content);
			mini->env_iter = mini->env_iter->next;
		}
		return ;
	}
	i = 1;
	while (args->args[i])
	{
		if (ft_strchr(args->args[i], '='))
			ft_export_env(args->args[i], mini);
		i++;
	}
}
