#include "codexion.h"

/*
Explications du scheduler

Le scheduler donne comment la stack de chaque dongle est remplie

Lorsqu'un coder demande a s'ajouter dans la stack.

Si il est en fifo

-> on l'ajoute en bas de la stack

Si il est en edf.

On compare son temps par rapport a celui des autres et on le met a la bonne place.

Lorsqu'un coder veut prendre le dongle. Il doit vérifier si il est available.

-> C A D si.
Avalable = 1
Et Si
temps mtnt - temps last use > temps de recharge
*/

void	push_back(t_coder *coder, t_heap **heap)
{
	t_heap	*to_add;
	t_heap	*work;

	work = *heap;
	to_add = malloc(sizeof(t_heap));
	if (to_add == NULL)
		return ;
	to_add->next = NULL;
	to_add->rank = coder->rank;
	if (*heap == NULL)
	{
		*heap = to_add;
		return ;
	}
	while (work->next != NULL)
		work = work->next;
	work->next = to_add;
}

void	pop(t_heap **heap)
{
	t_heap *to_remove;

	to_remove = *heap;
	*heap = (*heap)->next;
	free(to_remove);
}

void	fifo(t_coder *coder, t_dongle *dongle)
{
	push_back(coder, &dongle->waiting_list);
}

void	edf()
{

}

// Return 1 si coder déjà dans waiting list
// 0 si non
int	is_in_waitinglist(t_coder *coder, t_heap *waiting_list)
{
	while(waiting_list != NULL)
	{
		if (coder->rank == waiting_list->rank)
			return (1);
		waiting_list = waiting_list->next;
	}
	return (0);
}

// Ajoute a la file d'attente du dongle
// si il est pas déjà dessus
void	scheduler(t_coder *coder, t_dongle *dongle)
{	
	if (is_in_waitinglist(coder, dongle->waiting_list))
		return ;
	if (strcmp(coder->args.scheduler,"fifo") == 0)
		fifo(coder, dongle);
	else if (strcmp(coder->args.scheduler,"edf") == 0)
		edf(coder, dongle);
}