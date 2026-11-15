/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrach <hrach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:06 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/15 20:48:20 by hrach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle(t_dongle *d)
{
	if (heap_init(&d->queue, 2))
		return (1);
	d->in_use = 0;
	d->free_at = 0;
	return (0);
}

void	release_dongles(t_coder *c)
{
	long	free_at;

	free_at = get_time() + c->sim->cfg.dongle_cooldown;
	pthread_mutex_lock(&c->sim->sched_mutex);
	c->left->in_use = 0;
	c->right->in_use = 0;
	c->left->free_at = free_at;
	c->right->free_at = free_at;
	pthread_cond_broadcast(&c->sim->sched_cond);
	pthread_mutex_unlock(&c->sim->sched_mutex);
}
