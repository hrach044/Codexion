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

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;
	int		i;

	if (check_args(argc, argv))
	{
		fprintf(stderr, "Error: invalid arguments\n");
		return (1);
	}
	fill_config(&sim.cfg, argv);
	if (init_sim(&sim))
		return (1);
	i = 0;
	while (i < sim.cfg.number_of_coders)
	{
		pthread_create(&sim.threads[i], NULL, coder_routine, &sim.coders[i]);
		i++;
	}
	i = 0;
	while (i < sim.cfg.number_of_coders)
	{
		pthread_join(sim.threads[i], NULL);
		i++;
	}
	destroy_sim(&sim);
	return (0);
}