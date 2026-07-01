#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# define N_ARGS 8
# define INT_MAX 2147483647

typedef struct s_args
{
	int		parsing_flag;
	int		number_of_coders;
	int		number_of_compiles_required;
	int		time_to_burnout;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		dongle_cooldown;
	char	*scheduler;

}	t_args;

t_args	parse_args(int argc, char **argv);

#endif