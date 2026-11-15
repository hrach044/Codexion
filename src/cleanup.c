/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrach <hrach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:01 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/15 20:48:58 by hrach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_sim(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->dongles_ready)
	{
		free(sim->dongles[i].queue.data);
		i++;
	}
	pthread_mutex_destroy(&sim->log_mutex);
	pthread_cond_destroy(&sim->sched_cond);
	pthread_mutex_destroy(&sim->sched_mutex);
	free(sim->dongles);
	free(sim->coders);
	free(sim->threads);
}
