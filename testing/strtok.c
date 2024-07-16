/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:55:26 by danjimen &        #+#    #+#             */
/*   Updated: 2024/07/16 14:05:44 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

char *my_strtok(char *str, const char *delim)
{
	static char *static_str = NULL; // Almacena la cadena entre llamadas
	int i = 0, j = 0;

	// Si str no es NULL, reinicia la cadena
	if (str != NULL) {
		static_str = str;
	} else {
		// Si no hay más tokens, devuelve NULL
		if (static_str == NULL) {
			return NULL;
		}
		str = static_str;
	}

	// Saltar delimitadores iniciales
	while (str[i] != '\0' && strchr(delim, str[i]) != NULL) {
		i++;
	}

	// Si alcanzamos el final de la cadena, no hay más tokens
	if (str[i] == '\0') {
		static_str = NULL;
		return NULL;
	}

	// Marcar el inicio del token
	char *start = &str[i];

	// Buscar el final del token
	while (str[i] != '\0' && strchr(delim, str[i]) == NULL) {
		i++;
	}

	// Si encontramos un delimitador, reemplazarlo por '\0' y avanzar el puntero estático
	if (str[i] != '\0') {
		str[i] = '\0';
		static_str = &str[i + 1];
	} else {
		// Si alcanzamos el final de la cadena, no hay más tokens
		static_str = NULL;
	}

	return start;
}

int main() {
	char str[] = "Hola, mundo. Este es un ejemplo.";
	char *token = my_strtok(str, " ,.");

	while (token != NULL) {
		printf("%s\n", token);
		token = my_strtok(NULL, " ,.");
	}

	return 0;
}