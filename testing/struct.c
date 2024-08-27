#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../include/libft/libft.h"

#define MAX_ARGS 100

typedef enum e_param_type
{
	CMD,
	INFILE,
	HERE_DOC,
	OUTFILE,
	APPEND,
	PIPE,
	PARAMS,
	BUILTING
}	t_param_type;

typedef struct s_line_cmd
{
	char				*content;
	int					argc;
	t_param_type		type;
	struct s_line_cmd	*next;
}	t_line_cmd;

/* static void	asign_params(t_args *args, int i)
{
	args->argc = i;
	args->params.argc = i;
	write(1, "ERROR\n", 6);
	//args->params->content = strdup(args->args[i]);
	//args->params->param_type = BUILTING;
} */

int	main(void)
{
	char	*args[MAX_ARGS];
	int		i;

	args[0] = "HOLA";
	args[1] = "ADIOS";
	args[2] = "PRUEBA";
	args[3] = "FIN";

	i = 0;
	while (i < 4)
	{
		printf("args[%i] ==> %s\n", i, args[i]);
		//asign_params(&args, i);
		//printf("args.params->argc ==> %i\n", args.params.argc);
		//printf("args.params->content ==> %s\n", args.params->content);
		//printf("args.params->param_type ==> %i\n", args.params->param_type);	
		i++;
	}
	return (0);
}