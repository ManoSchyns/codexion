#include "codexion.h"

int	main(int argc, char **argv)
{
	t_args	args;
	t_list_coder *list_coder;
	int is_dead;

	is_dead = 0;
	args = parse_args(argc - 1, argv + 1);
	if (!args.parsing_flag)
		return (1);

	list_coder = get_coders(args, &is_dead);
	if (list_coder == NULL)
	{
		printf("%s\n", "Erreur lors de l'allocation de memoire");
		return (1);
	}
	coder((void *)&(list_coder->coders[0]));
	
	free_all(list_coder);
}
