/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sched.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrach <hrach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/15 20:19:50 by hrach             #+#    #+#             */
/*   Updated: 2026/11/15 20:20:18 by hrach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	make_key(t_coder *c)
{
	if (c->sim->cfg.scheduler == 0)
		return (c->sim->seq++);
	return (c->last_start + c->sim->cfg.time_to_burnout);
}

static int	my_turn(t_coder *c)
{
	return (!c->left->in_use && !c->right->in_use
		&& c->left->queue.data[0].id == c->id
		&& c->right->queue.data[0].id == c->id);
}

static int	wait_turn(t_coder *c)
{
	while (!my_turn(c) && !is_stopped(c->sim))
		pthread_cond_wait(&c->sim->sched_cond, &c->sim->sched_mutex);
	return (!is_stopped(c->sim));
}

static long	free_time(t_coder *c)
{
	if (c->left->free_at > c->right->free_at)
		return (c->left->free_at);
	return (c->right->free_at);
}

int	acquire_dongles(t_coder *c)
{
	t_req	me;
	long	until;

	me.id = c->id;
	pthread_mutex_lock(&c->sim->sched_mutex);
	me.key = make_key(c);
	heap_push(&c->left->queue, me);
	heap_push(&c->right->queue, me);
	if (!wait_turn(c))
	{
		pthread_mutex_unlock(&c->sim->sched_mutex);
		return (0);
	}
	heap_pop(&c->left->queue);
	heap_pop(&c->right->queue);
	c->left->in_use = 1;
	c->right->in_use = 1;
	until = free_time(c);
	pthread_mutex_unlock(&c->sim->sched_mutex);
	if (until > get_time())
		msleep(until - get_time());
	return (1);
}
