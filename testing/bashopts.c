#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_enabled_options()
{
	char *bashopts = getenv("BASHOPTS");
	
	if (bashopts != NULL) {
		printf("BASHOPTS: %s\n", bashopts);
		
		// Opcionalmente, podrías convertir BASHOPTS en letras como lo hace 'echo $-'
		if (strstr(bashopts, "interactive") != NULL) {
			printf("i");
		}
		if (strstr(bashopts, "history") != NULL) {
			printf("h");
		}
		if (strstr(bashopts, "monitor") != NULL) {
			printf("m");
		}
		// Añade más opciones según sea necesario

		printf("\n");
	} else {
		printf("No BASHOPTS found.\n");
	}
}

int main()
{
	print_enabled_options();
	return 0;
}