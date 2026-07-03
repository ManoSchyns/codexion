#include "codexion.h"

// Set les dongles a gauche et a droite
void set_left_rigth(t_list_coder *list_coder)
{
	int i;
	int left;
	int number_code;

	i = 0;
	left = 0;
	number_code = list_coder->number_of_coders;
	while (i < number_code)
	{
		left = (i - 1 + number_code) % number_code;
		list_coder->coders[i].left = list_coder->coders[left].right;
		i ++;
	}
}

// Set les dongles
// return 0 en cas d'erreur de malloc
// retrun 1 si non
int set_dongles(t_list_coder *list_coder)
{
	int i;
	t_dongle	*dongle;

	i = 0;
	dongle = NULL;
	while (i < list_coder->number_of_coders)
	{
		dongle = malloc(sizeof(t_dongle));
		if (dongle == NULL)
			return (0);
		dongle->available = 1;
		pthread_mutex_init(&dongle->mutex, NULL);
		pthread_cond_init(&dongle->cond, NULL);
		list_coder->coders[i].right = dongle;
		i ++;
	}
	set_left_rigth(list_coder);
	return (1);
}

t_list_coder *get_coders(t_args args, int *is_dead, pthread_mutex_t *mutex_printf)
{
	int i;
	t_list_coder *list_coder;
	long	start_time;

	i = 0;
	start_time = get_time_ms();
	list_coder = NULL;
	list_coder = malloc(sizeof(t_list_coder));

	if (list_coder == NULL)
		return (NULL);

	list_coder->number_of_coders = args.number_of_coders;
	list_coder->coders = malloc(sizeof(t_coder) * (args.number_of_coders));
	if (list_coder->coders == NULL)
	{
		free(list_coder);
		return (NULL);
	}

	while(i < list_coder->number_of_coders)
	{
		list_coder->coders[i].rank = i + 1;
		list_coder->coders[i].args = args;
		list_coder->coders[i].last_compile_start = start_time;
		list_coder->coders[i].start_time = start_time;
		list_coder->coders[i].is_dead = is_dead;
		list_coder->coders[i].is_done = 0;
		list_coder->coders[i].mutex_printf = mutex_printf;
		pthread_mutex_init(&list_coder->coders[i].mutex_is_dead, NULL);
		pthread_mutex_init(&list_coder->coders[i].mutex_is_done, NULL);
		pthread_mutex_init(&list_coder->coders[i].mutex_last_compile_start, NULL);
		i ++;
	}
	list_coder->is_dead = is_dead;
	list_coder->start_time = start_time;
	return list_coder;
}