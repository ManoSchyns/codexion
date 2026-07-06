#include "codexion.h"

// Return 1 si coder déjà dans waiting list
// 0 si non
int	is_in_waitinglist(t_coder *coder, t_heap *waiting_list)
{
	while (waiting_list != NULL)
	{
		if (coder->rank == waiting_list->rank)
			return (1);
		waiting_list = waiting_list->next;
	}
	return (0);
}

// Retire le premier element de la heap
void	pop(t_heap **heap)
{
	t_heap	*to_remove;

	if (*heap == NULL)
		return ;
	to_remove = *heap;
	*heap = (*heap)->next;
	free(to_remove);
}

// Ajoute a la file d'attente du dongle
// si il est pas déjà dessus
void	scheduler(t_coder *coder, t_dongle *dongle)
{
	if (strcmp(coder->args.scheduler, "fifo") == 0)
		ft_fifo(coder, dongle);
	else if (strcmp(coder->args.scheduler, "edf") == 0)
		edf(coder, dongle);
}
