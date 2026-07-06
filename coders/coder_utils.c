#include "codexion.h"

// Return 1 si le dongle est prenable
// Return 0 si non
int	dongle_is_available(t_coder *coder, t_dongle *dongle)
{
	long	now;
	long	dongle_cooldown;

	now = get_time_ms();
	dongle_cooldown = coder->args.dongle_cooldown;
	if (dongle->waiting_list == NULL)
		return (0);
	if (dongle->available && now - dongle->last_release >= dongle_cooldown
		&& dongle->waiting_list->rank == coder->rank)
		return (1);
	return (0);
}

// Prend un dongle
// Le retire la waiting list
void	taking_dongle(t_dongle *dongle)
{
	dongle->available = 0;
	pop(&dongle->waiting_list);
}

// Attend que le dongle souhaité soit libre
// Verifie combien de temps attendre minimum.
// Attend ce temps là ou que quelqu'un en libere un
void	waiting_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;
	long			delay;

	pthread_mutex_lock(&dongle->mutex);
	scheduler(coder, dongle);
	while (!dongle_is_available(coder, dongle) && !check_is_dead(coder))
	{
		delay = (dongle->last_release
				+ coder->args.dongle_cooldown - get_time_ms());
		if (delay < 0)
			delay = 0;
		ts = get_timeout_ms(delay);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
	}
	if (!check_is_dead(coder))
		taking_dongle(dongle);
	pthread_mutex_unlock(&dongle->mutex);
}

// Libere les 2 dongles utilisés.
// Broadcast qu'ils sont libérés
void	set_dongle_available(t_coder *coder)
{
	long	time;

	time = get_time_ms();
	pthread_mutex_lock(&coder->right->mutex);
	coder->right->available = 1;
	coder->right->last_release = time;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->mutex);
	pthread_mutex_lock(&coder->left->mutex);
	coder->left->available = 1;
	coder->left->last_release = time;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->mutex);
}

// Essaies de prendre 2 dongles
void	getting_dongles(t_coder *coder)
{
	if ((coder->rank == 1 || coder->args.number_of_coders
			== coder->rank) && coder->rank != 2
			&& !check_is_dead(coder))
	{
		waiting_dongle(coder, coder->left);
		if (check_is_dead(coder))
			return ;
		waiting_dongle(coder, coder->right);
	}
	else if (!check_is_dead(coder))
	{
		waiting_dongle(coder, coder->right);
		if (check_is_dead(coder))
			return ;
		waiting_dongle(coder, coder->left);
	}
	if (!check_is_dead(coder))
	{
		show_message(coder, "has taken a dongle");
		show_message(coder, "has taken a dongle");
	}
}
