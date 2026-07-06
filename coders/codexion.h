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

typedef struct s_heap t_heap;
//
typedef struct s_heap
{
	int		rank;
	long	start_waiting;
	long	deadline;
	t_heap	*next;
}	t_heap;

// A Dongle
typedef struct s_dongle
{
	int				available;
	long			last_release;
	t_heap			*waiting_list;
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
}	t_dongle;

// a codeur
typedef struct s_coder
{
	t_args			args;
	int				rank;
	long			last_compile_start;
	long			start_time;
	long			start_waiting;
	int				*is_dead;
	int				is_done;
	pthread_mutex_t	mutex_is_dead;
	pthread_mutex_t	mutex_is_done;
	pthread_mutex_t	mutex_last_compile_start;
	pthread_mutex_t	*mutex_printf;
	pthread_t		thread;
	t_dongle		*left;
	t_dongle		*right;
}	t_coder;

// A coder list
typedef struct s_list_coder
{
	t_coder		*coders;
	int			number_of_coders;
	int			*is_dead;
	long		start_time;
	pthread_t	monitor;
}	t_list_coder;

// Arguments for the monitor
typedef struct s_monitor
{
	int				i;
	t_list_coder	*list_coder;
	t_coder			*coder;
	long			time_to_burn;
	int				is_dead;
	int				is_done;
	int				count_done;
	long			last_compile_start;
}	t_monitor;

typedef struct s_setup
{
	t_args			args;
	int				i;
	long			start_time;
	pthread_mutex_t	*mutex_printf;
}	t_setup;

// parsing
t_args			parse_args(int argc, char **argv);

// coder thread
void			*coder(void *arg);

// Fullfill structure
t_list_coder	*get_coders(t_args args, int *is_dead,
					pthread_mutex_t *mutex_printf);
void			set_left_rigth(t_list_coder *list_coder);
int				set_dongles(t_list_coder *list_coder);
void			start_threads(t_list_coder *list_coder);
void			join_threads(t_list_coder *list_coder);

//utils
void			free_all(t_list_coder *list_coder);
long			get_time_ms(void);

// Monitor
void			*monitor(void *args);
void			set_safe_dead(t_monitor *datas);
void			set_safe_last_compile(t_monitor *datas);
void			print_safe_dead(t_monitor *datas);
void			get_safe_is_done(t_monitor *datas);

//scheduler
void			scheduler(t_coder *coder, t_dongle *dongle);
void			pop(t_heap **heap);
int				is_in_waitinglist(t_coder *coder, t_heap *waiting_list);
void			ft_fifo(t_coder *coder, t_dongle *dongle);
void			push_back(t_coder *coder, t_heap **heap);

#endif