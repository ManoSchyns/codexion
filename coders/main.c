#include "codexion.h"

int	main(int argc, char **argv)
{
	int				is_dead;
	t_args			args;
	t_list_coder	*list_coder;
	t_main			mutexes;

	is_dead = 0;
	args = parse_args(argc - 1, argv + 1);
	pthread_mutex_init(&mutexes.mutex_printf, NULL);
	pthread_mutex_init(&mutexes.mutex_is_dead, NULL);
	if (!args.parsing_flag)
		return (1);
	if (!get_coders_dongles(&list_coder, args, &is_dead, &mutexes))
		return (1);
	start_threads(list_coder);
	join_threads(list_coder);
	free_all(list_coder);
	pthread_mutex_destroy(&mutexes.mutex_printf);
	pthread_mutex_destroy(&mutexes.mutex_is_dead);
}
