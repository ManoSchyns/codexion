#include "codexion.h"

void	set_safe_dead(t_monitor *datas)
{
	pthread_mutex_lock(datas->coder->mutex_is_dead);
	*(datas->list_coder->is_dead) = 1;
	pthread_mutex_unlock(datas->coder->mutex_is_dead);
}

void	set_safe_last_compile(t_monitor *datas)
{
	pthread_mutex_lock(&datas->coder->mutex_last_compile_start);
	datas->last_compile_start = datas->coder->last_compile_start;
	pthread_mutex_unlock(&datas->coder->mutex_last_compile_start);
}

void	print_safe_dead(t_monitor *datas)
{
	pthread_mutex_lock(datas->coder->mutex_printf);
	printf("%ld %d burned out\n", get_time_ms()
		- datas->coder->start_time, datas->coder->rank);
	pthread_mutex_unlock(datas->coder->mutex_printf);
}

void	get_safe_is_done(t_monitor *datas)
{
	pthread_mutex_lock(&datas->coder->mutex_is_done);
	datas->is_done = datas->coder->is_done;
	pthread_mutex_unlock(&datas->coder->mutex_is_done);
}
