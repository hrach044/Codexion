/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:17 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:09 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->cfg.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static int	all_done(t_sim *sim)
{
	int	i;
	int	done;

	done = 1;
	pthread_mutex_lock(&sim->log_mutex);
	i = 0;
	while (i < sim->cfg.number_of_coders)
	{
		if (sim->coders[i].compiles < sim->cfg.number_of_compiles_required)
			done = 0;
		i++;
	}
	if (done)
		sim->stop = 1;
	pthread_mutex_unlock(&sim->log_mutex);
	return (done);
}

static int	check_coder(t_sim *sim, int i)
{
	long	now;
	int		dead;

	pthread_mutex_lock(&sim->log_mutex);
	now = get_time();
	dead = (now - sim->coders[i].last_start > sim->cfg.time_to_burnout);
	if (dead)
	{
		sim->stop = 1;
		printf("%ld %d burned out\n", now - sim->start_time, i + 1);
	}
	pthread_mutex_unlock(&sim->log_mutex);
	return (dead);
}

static int	scan(t_sim *sim)
{
	int	i;

	if (all_done(sim))
		return (1);
	i = 0;
	while (i < sim->cfg.number_of_coders)
	{
		if (check_coder(sim, i))
			return (1);
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!scan(sim))
		usleep(1000);
	wake_all(sim);
	return (NULL);
}
