/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:10 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:15 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_sim(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->cfg.number_of_coders;
	sim->dongles = malloc(sizeof(t_dongle) * n);
	sim->coders = malloc(sizeof(t_coder) * n);
	sim->threads = malloc(sizeof(pthread_t) * n);
	if (!sim->dongles || !sim->coders || !sim->threads)
	{
		free(sim->dongles);
		free(sim->coders);
		free(sim->threads);
		return (1);
	}
	pthread_mutex_init(&sim->log_mutex, NULL);
	sim->stop = 0;
	i = 0;
	sim->start_time = get_time();
	while (i < n)
	{
		if (init_dongle(&sim->dongles[i]))
			return (1);
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n];
		sim->coders[i].last_start = sim->start_time;
		i++;
	}
	return (0);
}