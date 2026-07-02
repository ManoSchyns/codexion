#include "codexion.h"

t_list_coder *get_coders(t_args args, int *is_dead)
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
		list_coder->coders[i].compilation_count = 0;
		list_coder->coders[i].is_done = 0;
		i ++;
	}
	list_coder->is_dead = is_dead;
	list_coder->start_time = start_time;
	return list_coder;
}

void free_all(t_list_coder *list_coder)
{
	free(list_coder->coders);
	free(list_coder);
}

long get_time_ms(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}