/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:28:34 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/16 13:55:10 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

int	main(int argc, char const *argv[])
{
	int	i;

	if (argc == 1)
		return (1);
	else
	{
		i = 1;
		while (i < argc)
		{
			printf("%i ==> %s\n", i, argv[i]);
			i++;
		}
	}
	return (0);
}
