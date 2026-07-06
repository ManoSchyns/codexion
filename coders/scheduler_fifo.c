#include "codexion.h"

// Logique fifo
void	ft_fifo(t_coder *coder, t_dongle *dongle)
{
	if (is_in_waitinglist(coder, dongle->waiting_list))
		return ;
	push_back(coder, &dongle->waiting_list);
	pthread_cond_broadcast(&dongle->cond);
}

// Push dans la heap le coder
// En fonction de son temps d'attente
void	push_back(t_coder *coder, t_heap **heap)
{
	t_heap	*to_add;
	t_heap	*cur;

	to_add = malloc(sizeof(t_heap));
	if (to_add == NULL)
		return ;
	to_add->rank = coder->rank;
	to_add->start_waiting = coder->start_waiting;
	to_add->next = NULL;
	if (*heap == NULL || to_add->start_waiting < (*heap)->start_waiting)
	{
		to_add->next = *heap;
		*heap = to_add;
		return ;
	}
	cur = *heap;
	while (cur->next && cur->next->start_waiting <= to_add->start_waiting)
		cur = cur->next;
	to_add->next = cur->next;
	cur->next = to_add;
}
