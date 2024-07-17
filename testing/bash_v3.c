/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_v3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:29:59 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/17 14:30:15 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ARGS 100

void parse_input(char *input, char *args[], int *argc)
{
	int in_single_quote = 0, in_double_quote = 0;
	char *arg = malloc(strlen(input) + 1);
	char *p = input, *q = arg;
	*argc = 0;

	while (*p) {
		if (*p == '\'' && !in_double_quote) {
			in_single_quote = !in_single_quote;
		} else if (*p == '\"' && !in_single_quote) {
			in_double_quote = !in_double_quote;
		} else if (isspace(*p) && !in_single_quote && !in_double_quote) {
			if (q != arg) {
				*q = '\0';
				args[(*argc)++] = strdup(arg);
				q = arg;
			}
		} else {
			*q++ = *p;
		}
		p++;
	}
	if (q != arg) {
		*q = '\0';
		args[(*argc)++] = strdup(arg);
	}

	free(arg);
}

int main()
{
	char input[1024];
	char *args[MAX_ARGS];
	int argc, i;

	printf("Enter a command: ");
	if (!fgets(input, sizeof(input), stdin)) {
		perror("fgets");
		return 1;
	}

	// Remove the newline character if present
	input[strcspn(input, "\n")] = 0;

	parse_input(input, args, &argc);

	printf("Parsed arguments:\n");
	for (i = 0; i < argc; i++) {
		printf("arg[%d]: %s\n", i, args[i]);
		free(args[i]);
	}

	return 0;
}
