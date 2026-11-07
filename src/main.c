/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:15 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 19:28:49 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_config	cfg;

	if (check_args(argc, argv))
	{
		fprintf(stderr, "Error: invalid arguments\n");
		return (1);
	}
	fill_config(&cfg, argv);
	printf("coders=%d burnout=%d sched=%d\n",
		cfg.number_of_coders, cfg.time_to_burnout, cfg.scheduler);
	return (0);
}