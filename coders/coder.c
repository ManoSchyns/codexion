#include "codexion.h"

void *coder(void *args)
{
	t_coder *coder = (t_coder *) args;

	while (!*(coder->is_dead) && coder->compilation_count < coder->args.number_of_compiles_required)
	{
	
		//ask 2 dongle

		//demander le -1 si disponible et si c'est a nottre tour
		// Take 2 dongles
		// On se fait POP
		//si 2 dongle -> Start compiling
		// compile
		// Quand on commence a compiler on met le dernier temps de compilation effectué
		coder->last_compile_start = get_time_ms();
		printf("%ld %d is compiling\n", get_time_ms() - coder->start_time, coder->rank);
		usleep(coder->args.time_to_compile);
		if (!*(coder->is_dead))
			return (NULL);
		coder->compilation_count += 1;

		//debug
		printf("%ld %d is debugging\n", get_time_ms() - coder->start_time, coder->rank);
		usleep(coder->args.time_to_debug);

		if (!*(coder->is_dead))
			return (NULL);

		//refactor
		printf("%ld %d is refactoring\n", get_time_ms() - coder->start_time, coder->rank);
		usleep(coder->args.time_to_refactor); 

	}
	return (NULL);
}