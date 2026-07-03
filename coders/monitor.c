#include "codexion.h"

void	analyse_one_coder(t_monitor *datas)
{
	set_safe_last_compile(datas);
	if (get_time_ms() - datas->last_compile_start
		>= datas->time_to_burn)
	{
		set_safe_dead(datas);
		datas->is_dead = 1;
		print_safe_dead(datas);
	}
}

void	*monitor(void *args)
{
	t_monitor	datas;

	datas.list_coder = (t_list_coder *)args;
	datas.is_dead = 0;
	datas.count_done = 0;
	datas.time_to_burn = (long)datas.list_coder->coders[0].args.time_to_burnout;
	while (datas.is_dead == 0 && datas.count_done
		< datas.list_coder->number_of_coders)
	{
		datas.i = 0;
		while (datas.i < datas.list_coder->number_of_coders
			&& datas.is_dead == 0)
		{
			datas.coder = &datas.list_coder->coders[datas.i];
			get_safe_is_done(&datas);
			if (!datas.is_done)
			{
				analyse_one_coder(&datas);
			}
			else
				datas.count_done ++;
			datas.i ++;
		}
	}
	return (NULL);
}
