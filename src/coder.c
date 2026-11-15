/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:04 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:20 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	take(t_coder *c, t_dongle *d)
{
	take_dongle(d, c);
	print_log(c->sim, c->id, "has taken a dongle");
}

static void take_two(t_coder *c)
{
	if (c->left < c->right)
	{
		take(c, c->left);
		take(c, c->right);
	}
	else
	{
		take(c, c->right);
		take(c, c->left);
	}
}

static void	mark_start(t_coder *c)
{
	pthread_mutex_lock(&c->sim->log_mutex);
	c->last_start = get_time();
	pthread_mutex_unlock(&c->sim->log_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder *coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		take_two(coder);
		mark_start(coder);
		print_log(coder->sim, coder->id, "is compiling");
		msleep(coder->sim->cfg.time_to_compile);
		release_dongle(coder->left, coder->sim->cfg.dongle_cooldown);
		release_dongle(coder->right, coder->sim->cfg.dongle_cooldown);
		print_log(coder->sim, coder->id, "is debugging");
		msleep(coder->sim->cfg.time_to_debug);
		print_log(coder->sim, coder->id, "is refactoring");
		msleep(coder->sim->cfg.time_to_refactor);
	}
	return (NULL);
}