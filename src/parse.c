/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 14:15:16 by danjimen          #+#    #+#             */
/*   Updated: 2024/07/15 10:46:38 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	analize_parts(char *input)
{
	analize_simple_quotes();
	analize_double_quotes();
	analize_infile();
	analize_outfile();
	analize_built_in();
}

int	parse(char *input)
{
	char	**split;
	t_mini	mini;

	ft_bzero(&mini, sizeof(t_mini));
	analize_parts(input);
	//input = analize_comillas(input);
	printf("You entered: %s\n", input);
	split = ft_split(input, '|');
	int i = 0;
	while (split[i])
	{
		printf("split[%d] = %s\n", i, split[i]);
		i++;
	}
	return (0);
}
/*
command < infile | command > outfile
command < infile
expoort empieza por letra= algo
*/