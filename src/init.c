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
	sim->dongles_ready = 0;
	sim->stop = 0;
	pthread_mutex_init(&sim->log_mutex, NULL);
	sim->dongles = malloc(sizeof(t_dongle) * n);
	sim->coders = malloc(sizeof(t_coder) * n);
	sim->threads = malloc(sizeof(pthread_t) * n);
	if (!sim->dongles || !sim->coders || !sim->threads)
		return (destroy_sim(sim), 1);
	sim->start_time = get_time();
	i = 0;
	while (i < n)
	{
		if (init_dongle(&sim->dongles[i]))
			return (destroy_sim(sim), 1);
		sim->dongles_ready++;
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].left = &sim->dongles[i];
		sim->coders[i].right = &sim->dongles[(i + 1) % n];
		sim->coders[i].last_start = sim->start_time;
		sim->coders[i].compiles = 0;
		i++;
	}
	return (0);
}