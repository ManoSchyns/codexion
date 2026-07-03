#include "codexion.h"

void	taking_dongle()
{
	
}


int is_dead(t_coder *coder)
{
	int is_dead;

	pthread_mutex_lock(&coder->mutex_is_dead);
	is_dead = *(coder->is_dead);
	pthread_mutex_unlock(&coder->mutex_is_dead);

	if (is_dead)
		return (1);
	return (0);
}

void show_message(t_coder *coder, char *message)
{
	pthread_mutex_lock(coder->mutex_printf);
	printf("%ld %d %s\n", get_time_ms() - coder->start_time, coder->rank, message);
	pthread_mutex_unlock(coder->mutex_printf);
}

void *coder(void *args)
{
	t_coder *coder = (t_coder *) args;
	int compilation_count;

	compilation_count = 0;
	while (!is_dead(coder))
	{
		if (compilation_count >= coder->args.number_of_compiles_required)
		{
			pthread_mutex_lock(&coder->mutex_is_done);
			coder->is_done = 1;
			pthread_mutex_unlock(&coder->mutex_is_done);
			return (NULL);
		}
		//ask 2 dongle

		//demander le -1 si disponible et si c'est a nottre tour
		// Take 2 dongles
		// On se fait POP
		//si 2 dongle -> Start compiling
		// compile
		// Quand on commence a compiler on met le dernier temps de compilation effectué
		pthread_mutex_lock(&coder->mutex_last_compile_start);
		coder->last_compile_start = get_time_ms();
		pthread_mutex_unlock(&coder->mutex_last_compile_start);

		if (is_dead(coder))
			return (NULL);
		show_message(coder, "is compiling");
		usleep(coder->args.time_to_compile * 1000);
	
		compilation_count += 1;

		if (is_dead(coder))
			return (NULL);
		//debug
		show_message(coder, "is debugging");
		usleep(coder->args.time_to_debug * 1000);

		if (is_dead(coder))
			return (NULL);

		//refactor
		show_message(coder, "is refactoring");
		usleep(coder->args.time_to_refactor * 1000); 

	}
	return (NULL);
}