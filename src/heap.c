/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:08 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:17 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *h, int capacity)
{
	h->data = malloc(sizeof(t_req) * capacity);
	h->size = 0;
	if (!h->data)
		return (1);
	return (0);
}

int	req_less(t_req a, t_req b)
{
	if (a.key != b.key)
		return (a.key < b.key);
	return (a.id < b.id);
}

void	heap_push(t_heap *h, t_req r)
{
	int		i;
	int		parent;
	t_req	tmp;

	i = h->size;
	h->data[i] = r;
	h->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!req_less(h->data[i], h->data[parent]))
			break ;
		tmp = h->data[i];
		h->data[i] = h->data[parent];
		h->data[parent] = tmp;
		i = parent;
	}
}