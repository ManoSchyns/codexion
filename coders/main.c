#include "codexion.h"

int	get_coders_dongles(t_list_coder	**list_coder, t_args args, int *is_dead,
						pthread_mutex_t *mutex_printf)
{
	*list_coder = get_coders(args, is_dead, mutex_printf);
	if (*list_coder == NULL)
	{
		printf("%s\n", "Erreur lors de l'allocation de memoire");
		return (0);
	}
	if (!set_dongles(*list_coder))
	{
		free_all(*list_coder);
		printf("%s\n", "Erreur lors de l'allocation de memoire");
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	int				is_dead;
	t_args			args;
	t_list_coder	*list_coder;
	pthread_mutex_t	mutex_printf;

	is_dead = 0;
	args = parse_args(argc - 1, argv + 1);
	pthread_mutex_init(&mutex_printf, NULL);
	if (!args.parsing_flag)
		return (1);
	get_coders_dongles(&list_coder, args, &is_dead, &mutex_printf);
	start_threads(list_coder);
	join_threads(list_coder);
	free_all(list_coder);
	pthread_mutex_destroy(&mutex_printf);
}
