/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_argv.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen <danjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 12:28:34 by danjimen &        #+#    #+#             */
/*   Updated: 2024/08/23 12:00:53 by danjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

//   
//  
//  && 
//  1 1 --> 1
//  1 0 --> 0
//  0 1 --> 0
//  0 0 --> 0
// 
// ||
//  1 1 --> 1
//  1 0 --> 1
//  0 1 --> 1
//  0 0 --> 0
// 
// XOR
//  1 1 -->  0
//  1 0 -->  1
//  0 1 -->  1
//  0 0 -->  0
// 
//static int doublequotes;
//--> Holaa       como 1es   tas    dsadadaasddss"


#include <string.h>

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
			int j = 0;
			while (argv[i][j])
			{
				printf("Char = %d\n", argv[i][j]);
				j++;
			}
			i++;
		}
	}
	return (0);
}
