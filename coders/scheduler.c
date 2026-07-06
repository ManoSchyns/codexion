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
void	print_hep(t_heap *heap)
{
	printf("========= MA HEAP ==========\n");
	while (heap != NULL)
	{
		printf("%d\n", heap->rank);
		heap = heap->next;
	}
	printf("\n========= End ==========\n");
}

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

void	pop(t_heap **heap)
{
	t_heap	*to_remove;

	if (*heap == NULL)
		return ;
	to_remove = *heap;
	*heap = (*heap)->next;
	free(to_remove);
}

void	edf()
{

}

// Ajoute a la file d'attente du dongle
// si il est pas déjà dessus
void	scheduler(t_coder *coder, t_dongle *dongle)
{	
	if (strcmp (coder->args.scheduler,"fifo") == 0)
		ft_fifo(coder, dongle);
	else if (strcmp(coder->args.scheduler,"edf") == 0)
	{
		pthread_mutex_lock(&coder->right->mutex);
		edf(coder, coder->right);
		pthread_mutex_unlock(&coder->right->mutex);	
		pthread_mutex_lock(&coder->left->mutex);
		edf(coder, coder->left);
		pthread_mutex_unlock(&coder->left->mutex);
	}
	//print_hep(dongle->waiting_list);
}