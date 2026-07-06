#include "codexion.h"

// Return le temps actuel + un delay
struct timespec	get_timeout_ms(long delay_ms)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = tv.tv_usec * 1000;
	ts.tv_nsec += delay_ms * 1000000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec += ts.tv_nsec / 1000000000;
		ts.tv_nsec %= 1000000000;
	}
	return (ts);
}

// affiche un message de manière sécurisée
void	show_message(t_coder *coder, char *message)
{
	pthread_mutex_lock(coder->mutex_printf);
	printf("%ld %d %s\n", get_time_ms()
		- coder->start_time, coder->rank, message);
	pthread_mutex_unlock(coder->mutex_printf);
}

// Verifie de maniere sécurisée si coder dead
int	check_is_dead(t_coder *coder)
{
	int	is_dead;

	is_dead = 0;
	pthread_mutex_lock(coder->mutex_is_dead);
	is_dead = *(coder->is_dead);
	pthread_mutex_unlock(coder->mutex_is_dead);
	if (is_dead)
		return (1);
	return (0);
}

// Return 1 si la routine s'est bien passéé
// 0 si non
int	routine(t_coder *coder, int *compilation_count)
{
	if (check_is_dead(coder))
		return (0);
	pthread_mutex_lock(&coder->mutex_last_compile_start);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex_last_compile_start);
	show_message(coder, "is compiling");
	usleep(coder->args.time_to_compile * 1000);
	set_dongle_available(coder);
	*compilation_count += 1;
	if (check_is_dead(coder))
		return (0);
	show_message(coder, "is debugging");
	usleep(coder->args.time_to_debug * 1000);
	if (check_is_dead(coder))
		return (0);
	show_message(coder, "is refactoring");
	usleep(coder->args.time_to_refactor * 1000);
	return (1);
}

// Thread d'un coder.
// Compile, debug et refactor n fois.
// Necessite des dongles pour compiler
void	*coder(void *args)
{
	t_coder	*coder;
	int		compilation_count;

	coder = (t_coder *) args;
	compilation_count = 0;
	while (!check_is_dead(coder))
	{
		if (compilation_count
			>= coder->args.number_of_compiles_required)
		{
			pthread_mutex_lock(&coder->mutex_is_done);
			coder->is_done = 1;
			pthread_mutex_unlock(&coder->mutex_is_done);
			return (NULL);
		}
		getting_dongles(coder);
		if (!routine(coder, &compilation_count))
			return (NULL);
	}
	return (NULL);
}
