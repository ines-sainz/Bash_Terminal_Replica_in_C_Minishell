/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_v3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:29:59 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/19 15:16:41 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/minishell.h"

#define MAX_ARGS 100

void	parse_input(char *input, char *args[], int *argc)
{
	int		in_single_quote;
	int		in_double_quote;
	char	*arg;
	char	*q;

	in_single_quote = 0;
	in_double_quote = 0;
	arg = malloc(strlen(input) + 1);
	q = arg;
	*argc = 0;

	while (*input)
	{
		if (*input == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*input == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (isspace(*input) && !in_single_quote && !in_double_quote)
		{
			if (q != arg)
			{
				*q = '\0';
				args[(*argc)++] = strdup(arg);
				q = arg;
			}
		}
		else
			*q++ = *input;
		input++;
	}
	if (q != arg)
	{
		*q = '\0';
		args[(*argc)++] = strdup(arg);
	}

	free(arg);
}

int	main(void)
{
	char	input[1024];
	char	*args[MAX_ARGS];
	int		argc;
	int		i;

	printf("Enter a command: ");
	if (!fgets(input, sizeof(input), stdin))
	{
		perror("fgets");
		return (1);
	}

	// Remove the newline character if present
	//input[strcspn(input, "\n")] = 0;
	input[strchr(input, '\n') - input] = 0;

	parse_input(input, args, &argc);

	printf("Parsed arguments:\n");
	i = 0;
	while (i < argc)
	{
		printf("arg[%d]: %s\n", i, args[i]);
		free(args[i]);
		i++;
	}

	return (0);
}
