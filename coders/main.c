#include "codexion.h"


int	main(int argc, char **argv)
{
	t_args	args;
	t_list_coder *list_coder;
	int is_dead;
	pthread_mutex_t mutex_printf;

	is_dead = 0;
	args = parse_args(argc - 1, argv + 1);
	pthread_mutex_init(&mutex_printf, NULL);
	if (!args.parsing_flag)
		return (1);

	list_coder = get_coders(args, &is_dead, &mutex_printf);
	if (list_coder == NULL)
	{
		printf("%s\n", "Erreur lors de l'allocation de memoire");
		return (1);
	}
	if (!set_dongles(list_coder))
	{
		printf("%s\n", "Erreur lors de l'allocation de memoire");
		return (1);
	}
	start_threads(list_coder);
	join_threads(list_coder);
	
	free_all(list_coder);
	pthread_mutex_destroy(&mutex_printf);
}
