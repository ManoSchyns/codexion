/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_fifo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschyns <mano.schyns@learner.42.tech>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 13:31:35 by mschyns           #+#    #+#             */
/*   Updated: 2026/07/22 13:31:36 by mschyns          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
void	push_back(t_coder *coder, t_heap **heap)
{
	t_heap	*to_add;
	t_heap	*cur;

	to_add = malloc(sizeof(t_heap));
	if (to_add == NULL)
		return ;
	to_add->rank = coder->rank;
	to_add->next = NULL;
	if (*heap == NULL )
	{
		to_add->next = *heap;
		*heap = to_add;
		return ;
	}
	cur = *heap;
	while (cur->next != NULL)
		cur = cur->next;
	to_add->next = cur->next;
	cur->next = to_add;
}
