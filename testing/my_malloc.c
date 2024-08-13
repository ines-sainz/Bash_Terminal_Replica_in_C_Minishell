#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Estructura para almacenar el tamaño del bloque
typedef struct {
	size_t size;
} BlockHeader;

void*	my_malloc(size_t size)
{
	// Reservar memoria para el bloque más el encabezado
	BlockHeader* header = (BlockHeader*)malloc(size + sizeof(BlockHeader));
	if (header == NULL)
		return (NULL);
	// Guardar el tamaño en el encabezado
	header->size = size;
	// Devolver un puntero al área de datos, después del encabezado
	return ((void*)(header + 1));
}

void	my_free(void* ptr)
{
	if (ptr == NULL) {
		return ;
	}
	// Obtener el puntero al encabezado y liberar la memoria
	BlockHeader* header = (BlockHeader*)ptr - 1;
	free(header);
}

size_t	my_malloc_usable_size(void* ptr)
{
	if (ptr == NULL)
		return (0);
	// Obtener el puntero al encabezado y devolver el tamaño almacenado
	BlockHeader* header = (BlockHeader*)ptr - 1;
	return (header->size);
}

void	*my_realloc(void* ptr, size_t new_size)
{
	size_t	old_size;

	// Si ptr es NULL, entonces realloc se comporta como malloc
	if (ptr == NULL)
		return (my_malloc(new_size));
	
	// Si new_size es 0, entonces realloc se comporta como free
	if (new_size == 0)
	{
		my_free(ptr);
		return (NULL);
	}

	// Asignar un nuevo bloque de memoria
	void* new_ptr = my_malloc(new_size);
	if (new_ptr == NULL)
		// Si malloc falla, se devuelve NULL, y el bloque original no se libera
		return (NULL);

	// Obtener el tamaño actual del bloque
	old_size = my_malloc_usable_size(ptr); // Esto es específico de algunas implementaciones de malloc
	// Copiar los datos del bloque viejo al nuevo
	if (old_size < new_size)
		memcpy(new_ptr, ptr, old_size);
	else
		memcpy(new_ptr, ptr, new_size);

	// Liberar el bloque viejo
	my_free(ptr);

	return (new_ptr);
}

int	main(void)
{
	// Ejemplo de uso
	void* ptr = my_malloc(100);
	printf("Usable size: %zu\n", my_malloc_usable_size(ptr)); // Debería imprimir 100
	ptr = my_realloc(ptr, 120);
	printf("Usable size: %zu\n", my_malloc_usable_size(ptr)); // Debería imprimir 120
	my_free(ptr);
	return (0);
}
