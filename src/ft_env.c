/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:42:02 by isainz-r          #+#    #+#             */
/*   Updated: 2024/07/18 09:55:26 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void	ft_print_env(t_mini *mini)
{
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter != NULL)
	{
		printf("%s=%s\n", mini->env_iter->variable, mini->env_iter->content);
		mini->env_iter = mini->env_iter->next;
	}
}

void	free_env(t_mini *mini)
{
	t_env	*next;

	if (mini->env_first_node)
	{
		mini->env_iter = mini->env_first_node;
		while (mini->env_iter != NULL)
			mini->env_iter = mini->env_first_node;
		while (mini->env_iter != NULL)
		{
			next = mini->env_iter;
			free(next->variable);
			free(next->content);
			free(next);
			mini->env_iter = mini->env_iter->next;
		}
		next = NULL;
	}
	mini->env_first_node = NULL;
	mini->env_iter = NULL;
}

int	add_back_env(t_env *node, t_mini *mini)
{
	if (!mini->env_first_node)
	{
		mini->env_first_node = node;
		return (0);
	}
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter->next != NULL)
		mini->env_iter = mini->env_iter->next;
	mini->env_iter->next = node;
	return (0);
}

t_env	*env_new(char *env)
{
	t_env	*node;
	int		i;
	int		pos_equal;

	i = 0;
	pos_equal = 0;
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	while (env[i])
	{
		if (env[i] == '=' && pos_equal == 0)
			pos_equal = i;
		i++;
	}
	node->variable = ft_substr(env, 0, pos_equal);
	node->content = ft_substr(env, pos_equal + 1, ft_strlen(env) - pos_equal);
	node->next = NULL;
	return (node);
}

//cuento con que solo me dan lla variable que quiere dessetear, no el contenido también
void	ft_unset_env(char *unset, t_mini *mini)
{
	t_env	*prev;

	while (mini->env_iter != NULL)
	{
		if (!ft_strncmp(unset, mini->env_iter->variable, ft_strlen(unset)))
		{
			if (!ft_strncmp(mini->env_iter->variable, mini->env_first_node->variable, ft_strlen(mini->env_first_node->variable)))
			{
				mini->env_first_node = mini->env_first_node->next;
				free(mini->env_iter->variable);
				free(mini->env_iter->content);
				free(mini->env_iter);
				return ;
			}
			prev = mini->env_first_node;
			while (prev != NULL && ft_strncmp(prev->next->variable, unset, ft_strlen(unset)))
				prev = prev->next;
			if (mini->env_iter->next)
				prev->next = mini->env_iter->next;
			else
				prev->next = NULL;
			free(mini->env_iter->variable);
			free(mini->env_iter->content);
			free(mini->env_iter);
			return ;
		}
		mini->env_iter = mini->env_iter->next;
	}
}

void	ft_export_env(char *new_env, t_mini *mini)
{
	t_env	*new_node;

	new_node = env_new(new_env);
	if (!new_node)
	{
		free_env(mini);
		return ;
	}
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter != NULL)
	{
		if (!ft_strncmp(new_node->variable, mini->env_iter->variable,
			ft_strlen(mini->env_iter->variable)))
		{
			mini->env_iter->content = ft_strdup(new_node->content);
			free(new_node->content);
			free(new_node->variable);
			free(new_node);
			return ;
		}
		mini->env_iter = mini->env_iter->next;
	}
	add_back_env(new_node, mini);
}

char	*ft_get_env(char *to_expand, t_mini *mini)
{
	mini->env_iter = mini->env_first_node;
	while (mini->env_iter != NULL)
	{
		if (!ft_strncmp(to_expand, mini->env_iter->variable, ft_strlen(mini->env_iter->variable)))
			return (mini->env_iter->content);
		mini->env_iter = mini->env_iter->next;
	}
	return (0);
}

int	ft_set_env(char **env, t_mini *mini)
{
	t_env	*node;
	int		i;

	i = 0;
	mini->env_first_node = NULL;
	while (env[i])
	{
		node = env_new(env[i]);
		if (!node)
			free_env(mini);
		i++;
		add_back_env(node, mini);
	}
	return (0);
}

/*int	main(int argc, char **argv, char **env)
{
	t_mini	mini;

	(void)argv;
	if (argc != 1)
		return (1);
	printf("/////////////////////////////////////\n         STETEA EL ENV              \n/////////////////////////////////////\n");
	ft_set_env(env, &mini);
	printf("/////////////////////////////////////\n         IMPRIME EL ENV              \n/////////////////////////////////////\n");
	ft_print_env(&mini);
	printf("/////////////////////////////////////\n         BUSCA LA VARIABLE              \n/////////////////////////////////////\n");
	printf("Devuelve: %s\n", ft_get_env("USER", &mini));
	printf("/////////////////////////////////////\n         EXPORTA LA VARIABLE              \n/////////////////////////////////////\n");
	ft_export_env("INES=ines", &mini);
	printf("/////////////////////////////////////\n         IMPRIME EL ENV              \n/////////////////////////////////////\n");
	ft_print_env(&mini);
	printf("/////////////////////////////////////\n      CAMBIA CONTENIDO DE VARIABLE       \n/////////////////////////////////////\n");
	ft_export_env("INES=iines", &mini);
	printf("/////////////////////////////////////\n         IMPRIME EL ENV              \n/////////////////////////////////////\n");
	ft_print_env(&mini);
	printf("/////////////////////////////////////\n         DESSTETEA LA VARIABLE              \n/////////////////////////////////////\n");
	ft_unset_env("INES", &mini);
	printf("/////////////////////////////////////\n         IMPRIME EL ENV              \n/////////////////////////////////////\n");
	ft_print_env(&mini);
}*/

/*
******************FALLA**********************
_=/home/isainz-r/Desktop/test_mini/./mini ******MIO
_=/usr/bin/env ******ORIGINAL
******************FALLA**********************
el unset no unset la variable.

$ echo $VAR
$var=
$ echo $VAR
$
*/