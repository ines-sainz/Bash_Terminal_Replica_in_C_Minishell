/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_built_ins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:31:32 by isainz-r          #+#    #+#             */
/*   Updated: 2024/07/30 09:31:35 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*join_four_args(char *var, char *equal, char *content, char *jump)
{
	int		len;
	char	*output;
	int		i;
	int		j;

	i = 0;
	j = 0;
	len = ft_strlen(var);
	len += ft_strlen(equal);
	len += ft_strlen(content);
	len += ft_strlen(jump);
	output = ft_calloc(len + 1, sizeof(1));
	while (var[j])
		output[i++] = var[j++];
	output[i++] = equal[0];
	j = 0;
	while (content[j])
		output[i++] = content[j++];
	j = 0;
	while (jump[j])
		output[i] = jump[j++];
	return (output);
}

char	*ft_built_env(t_mini *mini)
{
	char	*temp;
	char	*line;

	mini->env_iter = mini->env_first_node;
	line = "";
	while (mini->env_iter != NULL)
	{
		temp = ft_strdup(line);
		line = ft_strjoin(temp, join_four_args(mini->env_iter->variable, "=",
			mini->env_iter->content, "\n"));
		free(temp);
		mini->env_iter = mini->env_iter->next;
	}
	return (line);
}

char	*buit_ins(char *command, char *argv, t_mini *mini)
{
	char	*output;
	char	*buffer;
	char	*temp;

	output = NULL;
	buffer = NULL;
	if (!ft_strncmp("cd", command, ft_strlen(command)))
	{
		output = getcwd(buffer,  INT_MAX);
		/*if (!output)*/
		temp = ft_strjoin(output, "/");
		free(buffer);
		buffer = ft_strjoin(temp, argv);
		chdir(buffer);
		temp = ft_strjoin("OLDPWD=", output);
		free(output);
		output = NULL;
		ft_export_env(temp, mini);
		free(temp);
		temp = ft_strjoin("PWD=", buffer);
		ft_export_env(temp, mini);
		free(temp);
		rl_on_new_line();
		temp = ft_strjoin(getenv("USER"), "@minishell/");
		buffer = ft_strjoin(temp, argv);
		free(temp);
		temp = ft_strjoin(buffer, "> ");
		/*if (!temp)*/
		mini->user_prompt = malloc(ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(temp) + ft_strlen(RESET) + 1);
		ft_strcpy(mini->user_prompt, RED);
		ft_strlcat(mini->user_prompt, BOLD, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(temp) + ft_strlen(RESET) + 1);
		ft_strlcat(mini->user_prompt, temp, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(temp) + ft_strlen(RESET) + 1);
		ft_strlcat(mini->user_prompt, RESET, ft_strlen(RED) + ft_strlen(BOLD) + ft_strlen(temp) + ft_strlen(RESET) + 1);
		rl_replace_line(mini->user_prompt, 0);
		rl_redisplay();
	}
	if (!ft_strncmp("pwd", command, ft_strlen(command)))
		output = getcwd(buffer, INT_MAX);
	if (!ft_strncmp("export", command, ft_strlen(command)))
		ft_export_env(argv, mini);
	if (!ft_strncmp("env", command, ft_strlen(command)))
		output = ft_built_env(mini);
	return (output);
}
