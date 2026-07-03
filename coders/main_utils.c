#include "codexion.h"

void	free_all(t_list_coder *list_coder)
{
	int	i;

	i = 0;
	while (i < list_coder->number_of_coders)
	{
		pthread_cond_destroy(&list_coder->coders[i].right->cond);
		pthread_mutex_destroy(&list_coder->coders[i].right->mutex);
		pthread_mutex_destroy(&list_coder->coders[i].mutex_is_dead);
		pthread_mutex_destroy(&list_coder->coders[i].mutex_is_done);
		pthread_mutex_destroy(&list_coder->coders[i].mutex_last_compile_start);
		free(list_coder->coders[i].right);
		i ++;
	}
	free(list_coder->coders);
	free(list_coder);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	start_threads(t_list_coder *list_coder)
{
	int	i;

	i = 0;
	pthread_create(&list_coder->monitor, NULL, monitor, list_coder);
	while (i < list_coder->number_of_coders)
	{
		pthread_create(&list_coder->coders[i].thread, NULL,
			coder, &list_coder->coders[i]);
		i ++;
	}
}

void	join_threads(t_list_coder *list_coder)
{
	int	i;

	i = 0;
	pthread_join(list_coder->monitor, NULL);
	while (i < list_coder->number_of_coders)
	{
		pthread_join(list_coder->coders[i].thread, NULL);
		i ++;
	}
}
