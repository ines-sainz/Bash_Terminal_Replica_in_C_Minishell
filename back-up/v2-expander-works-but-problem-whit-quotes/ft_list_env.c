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
