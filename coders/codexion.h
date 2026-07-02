#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

# define N_ARGS 8
# define INT_MAX 2147483647

// les arguments
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

// Un codeur
typedef struct s_coder
{
	t_args	args;
	int		rank;
	long	last_compile_start;
	long	start_time;
	int		compilation_count;
	int		is_done;
	int *is_dead;
}	t_coder;

// une liste de coder
typedef struct s_list_coder
{
	t_coder	*coders;
	int		number_of_coders;
	int		*is_dead;
	long	start_time;
}	t_list_coder;

t_args	parse_args(int argc, char **argv);
void *coder(void *arg);
t_list_coder *get_coders(t_args args, int *is_dead);
void free_all(t_list_coder *list_coder);
long get_time_ms(void);

#endif