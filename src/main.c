/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:15 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 20:00:39 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(void)
{
	t_heap	h;
	t_req	r;
	int		i;

	heap_init(&h, 4);
	r.key = 30;
	r.id = 1;
	heap_push(&h, r);
	r.key = 10;
	r.id = 2;
	heap_push(&h, r);
	r.key = 20;
	r.id = 3;
	heap_push(&h, r);
	i = 0;
	while (i < 3)
	{
		printf("%d\n", heap_pop(&h).id);
		i++;
	}
	free(h.data);
	return (0);
}