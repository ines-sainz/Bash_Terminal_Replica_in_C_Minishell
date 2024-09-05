/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isainz-r <isainz-r@student.42madrid>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:19 by isainz-r          #+#    #+#             */
/*   Updated: 2024/09/05 12:25:21 by isainz-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	set_here_doc(t_params *iter_params, t_pipes *iter_pipes, t_mini *mini)
{
	//primero abre here_doc luego escribe que se ha roto
	if (iter_pipes->inf_pipe < 0 || iter_pipes->outf_pipe < 0)
		return ;
	if (iter_pipes->inf_pipe != 0)
		close(iter_pipes->inf_pipe);
	iter_pipes->inf_pipe = open(iter_params->content, O_RDONLY);
	if (iter_pipes->inf_pipe < 0)
	{
		printf("minishell: %s: No such file or directory\n",
			iter_params->content);
		ft_export_env("?=1", mini);
	}
}
