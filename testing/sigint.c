#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void funcion(int sig)
{
	printf("Hola inés: %i\n", sig);
}

int main(void)
{
	signal(SIGINT, funcion);
	int	i = 0;
	while (1)
	{
		if (i >= 10)
			signal(SIGINT, SIG_DFL);
		printf("Time => %d\n", i);
		sleep(1);
		i++;
	}
	return 0;
}
