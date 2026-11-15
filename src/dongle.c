/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:06 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:19 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_dongle(t_dongle *d)
{
	pthread_mutex_init(&d->mutex, NULL);
	pthread_cond_init(&d->cond, NULL);
	d->in_use = 0;
	d->free_at = 0;
	d->seq = 0;
	return (heap_init(&d->queue, 2));
}

void	take_dongle(t_dongle *d, t_coder *c)
{
	t_req	me;
	long	until;

	me.id = c->id;
	pthread_mutex_lock(&d->mutex);
	if (c->sim->cfg.scheduler == 0)
		me.key = d->seq++;
	else
		me.key = c->last_start + c->sim->cfg.time_to_burnout;
	heap_push(&d->queue, me);
	while (d->in_use || d->queue.data[0].id != c->id)
		pthread_cond_wait(&d->cond, &d->mutex);
	heap_pop(&d->queue);
	d->in_use = 1;
	until = d->free_at;
	pthread_mutex_unlock(&d->mutex);
	if (until > get_time())
		msleep(until - get_time());
}

void	release_dongle(t_dongle *d, long cooldown)
{
	pthread_mutex_lock(&d->mutex);
	d->in_use = 0;
	d->free_at = get_time() + cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}