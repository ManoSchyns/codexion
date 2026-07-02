#include "codexion.h"

void *monitor(void *args)
{
	int i;
	int number_done;
	t_list_coder *list_coder;
	t_coder coder;
	long time_to_burn;

	number_done = 0;
	while(!*(list_coder->is_dead) && number_done < list_coder->number_of_coders)
	{
		list_coder = (t_list_coder *)args;
		time_to_burn = (long)list_coder->coders[0].args.time_to_burnout;
		i = 0;
		while (i < list_coder->number_of_coders && !*(list_coder->is_dead) && number_done < list_coder->number_of_coders)
		{
			coder = list_coder->coders[i];
			if (!(coder.is_done) && coder.compilation_count >= coder.args.number_of_compiles_required)
			{
				number_done += 1;
				coder.is_done = 1;
			}
			else if (!(coder.is_done) && get_time_ms()- coder.last_compile_start >= time_to_burn)
			{
				printf("%ld %d burned out", get_time_ms() - coder.start_time, coder.rank);
				*(list_coder->is_dead) = 1;
			}
			i ++;
		}
	}
	return (NULL);
}