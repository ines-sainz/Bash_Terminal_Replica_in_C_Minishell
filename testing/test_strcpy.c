/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_strcpy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 08:06:00 by danjimen &        #+#    #+#             */
/*   Updated: 2024/10/09 08:06:00 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../include/libft/libft.h"

int	main(void)
{
	char	*str = "Esto es una frase";
	char	*str2 = "Tengo sueño";
	char	*str3 = "Y también hambre";
	char	dst[20];

	printf("str == %s\n", str);
	printf("str2 == %s\n", str2);
	printf("str3 == %s\n\n", str3);
	printf("dst == %s\n\n", dst);
	ft_strcpy(dst, str);
	printf("dst == %s\n\n", dst);
	ft_strcpy(dst, str2);
	printf("dst == %s\n\n", dst);
	ft_strcpy(dst, str3);
	printf("dst == %s\n\n", dst);
	return (0);
}
