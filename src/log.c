/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:13 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:13 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_log(t_sim *sim, int id, char *msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	if (!sim->stop)
		printf("%ld %d %s\n", get_time() - sim->start_time, id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

int	is_stopped(t_sim *sim)
{
	int	s;

	pthread_mutex_lock(&sim->log_mutex);
	s = sim->stop;
	pthread_mutex_unlock(&sim->log_mutex);
	return (s);
}

void	mark_start(t_coder *c)
{
	pthread_mutex_lock(&c->sim->log_mutex);
	c->last_start = get_time();
	pthread_mutex_unlock(&c->sim->log_mutex);
}

void	count_compile(t_coder *c)
{
	pthread_mutex_lock(&c->sim->log_mutex);
	c->compiles++;
	pthread_mutex_unlock(&c->sim->log_mutex);
}
