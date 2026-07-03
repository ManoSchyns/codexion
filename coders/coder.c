#include "codexion.h"

void	show_message(t_coder *coder, char *message)
{
	pthread_mutex_lock(coder->mutex_printf);
	printf("%ld %d %s\n", get_time_ms()
		- coder->start_time, coder->rank, message);
	pthread_mutex_unlock(coder->mutex_printf);
}

int	check_is_dead(t_coder *coder)
{
	int	is_dead;

	is_dead = 0;
	pthread_mutex_lock(&coder->mutex_is_dead);
	is_dead = *(coder->is_dead);
	pthread_mutex_unlock(&coder->mutex_is_dead);
	if (is_dead)
		return (1);
	return (0);
}

// Return 1 si le dongle est prenable
// Return 0 si non
int		dongle_is_available(t_coder *coder, t_dongle *dongle)
{
	long	now;
	long	dongle_cooldown;

	now = get_time_ms();
	dongle_cooldown = coder->args.dongle_cooldown;
	scheduler(coder, dongle);
	if (dongle->available && now - dongle->last_release >= dongle_cooldown
		&& dongle->waiting_list->rank == coder->rank)
		return (1);
	return (0);
}

void	taking_dongle(t_coder *coder, t_dongle *dongle)
{
	dongle->available = 0;
	show_message(coder, "has taken a dongle");
	pop(&dongle->waiting_list);
}

void waiting_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);

	while (!dongle_is_available(coder, dongle) && !check_is_dead(coder))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);

	if (!check_is_dead(coder))
		taking_dongle(coder, dongle);

	pthread_mutex_unlock(&dongle->mutex);
}

void set_dongle_available(t_coder *coder)
{
	long	time;

	time = get_time_ms();
	coder->right->available = 1;
	coder->right->last_release = time;
	coder->left->available = 1;
	coder->right->last_release = time;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_cond_broadcast(&coder->left->cond);
}

// Return 1 si la routine s'est bien passéé
// 0 si non
int	routine(t_coder *coder, int *compilation_count)
{
	pthread_mutex_lock(&coder->mutex_last_compile_start);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->mutex_last_compile_start);
	if (check_is_dead(coder))
		return (0);
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
		//ask 2 dongle

		//demander le -1 si disponible et si c'est a nottre tour
		// Take 2 dongles
		// On se fait POP
		//si 2 dongle -> Start compiling
		// compile
		// Quand on commence a compiler on met le dernier temps de compilation effectué
		waiting_dongle(coder, coder->right);
		waiting_dongle(coder, coder->left);
		if (!routine(coder, &compilation_count))
			return (NULL);

	}
	return (NULL);
}