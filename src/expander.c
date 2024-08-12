/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 09:07:57 by isainz-r          #+#    #+#             */
/*   Updated: 2024/08/12 22:31:31 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

//Sólo entra aquí el dolar y la palabra de después $....
char	*expander(char *variable)
{
	char	*expanded;

	if (variable[1] >= '0' && variable[1] <= '9')
		return (ft_substr(variable, 2, (ft_strlen(variable) - 2)));
	expanded = getenv(++variable);
	if (!expanded)
		return (ft_strdup("\n"));
	return (expanded);
}