/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_pid.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danjimen & isainz-r <danjimen & isainz-    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 11:26:05 by danjimen &        #+#    #+#             */
/*   Updated: 2024/09/04 14:26:14 by danjimen &       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int	get_pid_from_proc_self(void)
{
	int		pid;
	char	buffer[256];
	int		fd;
	int		i;
	int		bytes_read;

	pid = 0;
	i = 0;

	// Abre el archivo /proc/self/stat
	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (-1);
	}

	// Lee el contenido del archivo
	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read <= 0)
	{
		perror("read");
		close(fd);
		return (-1);
	}

	// Asegúrate de que el buffer esté terminado en NULL
	buffer[bytes_read] = '\0';

	// Cierra el archivo
	close(fd);

	// Recorre el buffer para extraer el PID (primer número)
	while (buffer[i] >= '0' && buffer[i] <= '9')
	{
		pid = pid * 10 + (buffer[i] - '0');
		i++;
	}

	return (pid);
}

int	main(void)
{
	int	pid;

	pid = get_pid_from_proc_self();
	sleep(5);
	printf("get_pid.c: TERMINÉ\n");
	if (pid != -1)
		printf("El PID es: %d\n", pid);
	return (0);
}