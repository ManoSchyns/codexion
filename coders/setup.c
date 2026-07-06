#include "codexion.h"

// Set les dongles a gauche et a droite
void	set_left_rigth(t_list_coder *list_coder)
{
	int	i;
	int	left;
	int	number_code;

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
int	set_dongles(t_list_coder *list_coder)
{
	int			i;
	t_dongle	*dongle;

	i = 0;
	dongle = NULL;
	while (i < list_coder->number_of_coders)
	{
		dongle = malloc(sizeof(t_dongle));
		if (dongle == NULL)
			return (0);
		dongle->available = 1;
		dongle->last_release = 0;
		dongle->waiting_list = NULL;
		pthread_mutex_init(&dongle->mutex, NULL);
		pthread_cond_init(&dongle->cond, NULL);
		list_coder->coders[i].right = dongle;
		dongle = NULL;
		i ++;
	}
	set_left_rigth(list_coder);
	return (1);
}

void	set_datas(t_list_coder *list_coder, t_setup *datas, int *is_dead,
					pthread_mutex_t *mutex_is_dead)
{
	list_coder->coders[datas->i].rank = datas->i + 1;
	list_coder->coders[datas->i].args = datas->args;
	list_coder->coders[datas->i].last_compile_start = datas->start_time;
	list_coder->coders[datas->i].start_time = datas->start_time;
	list_coder->coders[datas->i].is_dead = is_dead;
	list_coder->coders[datas->i].is_done = 0;
	list_coder->coders[datas->i].mutex_printf = datas->mutex_printf;
	list_coder->coders[datas->i].mutex_is_dead = mutex_is_dead;
	pthread_mutex_init(&list_coder->coders[datas->i].mutex_is_done, NULL);
	pthread_mutex_init(&list_coder->coders[datas->i].mutex_last_compile_start,
		NULL);
}

// Retourne la liste de coders avec tous les arguments
t_list_coder	*get_coders(t_args args,
	int *is_dead, pthread_mutex_t *mutex_printf, pthread_mutex_t *mutex_is_dead)
{
	t_setup			data;
	t_list_coder	*list_coder;

	data.i = 0;
	data.start_time = get_time_ms();
	data.args = args;
	data.mutex_printf = mutex_printf;
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
	while (data.i < list_coder->number_of_coders)
	{
		set_datas(list_coder, &data, is_dead, mutex_is_dead);
		data.i ++;
	}
	list_coder->is_dead = is_dead;
	list_coder->start_time = data.start_time;
	return (list_coder);
}

// Set coder et dongles
int	get_coders_dongles(t_list_coder	**list_coder, t_args args, int *is_dead,
						t_main *mutexes)
{
	*list_coder = get_coders(args, is_dead, &mutexes->mutex_printf,
			&mutexes->mutex_is_dead);
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
