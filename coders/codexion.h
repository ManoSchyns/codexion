#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

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

// Dongle
typedef struct s_dongle
{
	int available;
	pthread_mutex_t mutex;
    pthread_cond_t  cond;
}	t_dongle;

// Un codeur
typedef struct s_coder
{
	t_args	args;
	int		rank;
	long	last_compile_start;
	long	start_time;
	int *is_dead;
	int is_done;
	pthread_mutex_t mutex_is_dead;
	pthread_mutex_t mutex_is_done;
	pthread_mutex_t mutex_last_compile_start;
	pthread_mutex_t *mutex_printf;
	pthread_t   thread;
	t_dongle *left;
	t_dongle *right;
}	t_coder;

// une liste de coder
typedef struct s_list_coder
{
	t_coder	*coders;
	int		number_of_coders;
	int		*is_dead;
	long	start_time;
	pthread_t   monitor;
}	t_list_coder;

typedef struct s_monitor
{
	int i;
	t_list_coder *list_coder;
	t_coder *coder;
	long time_to_burn;
	int is_dead;
	int is_done;
	int count_done;
	long last_compile_start;
}	t_monitor;

t_args	parse_args(int argc, char **argv);
void *coder(void *arg);
t_list_coder *get_coders(t_args args, int *is_dead, pthread_mutex_t *mutex_printf);
void free_all(t_list_coder *list_coder);
long get_time_ms(void);
void set_left_rigth(t_list_coder *list_coder);
int set_dongles(t_list_coder *list_coder);
void start_threads(t_list_coder *list_coder);
void join_threads(t_list_coder *list_coder);

// Monitor
void *monitor(void *args);
void	set_safe_dead(t_monitor *datas);
void	set_safe_last_compile(t_monitor *datas);
void	print_safe_dead(t_monitor *datas);
void	get_safe_is_done(t_monitor *datas);


#endif