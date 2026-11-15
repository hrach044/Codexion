/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hrach <hrach@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:04 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/15 20:35:09 by hrach            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	lonely_coder(t_coder *c)
{
	print_log(c->sim, c->id, "has taken a dongle");
	while (!is_stopped(c->sim))
		usleep(1000);
}

static int	take_two(t_coder *c)
{
	if (c->left == c->right)
	{
		lonely_coder(c);
		return (0);
	}
	if (!acquire_dongles(c))
		return (0);
	print_log(c->sim, c->id, "has taken a dongle");
	print_log(c->sim, c->id, "has taken a dongle");
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!is_stopped(coder->sim))
	{
		if (!take_two(coder))
			break ;
		mark_start(coder);
		print_log(coder->sim, coder->id, "is compiling");
		msleep(coder->sim->cfg.time_to_compile);
		count_compile(coder);
		release_dongles(coder);
		print_log(coder->sim, coder->id, "is debugging");
		msleep(coder->sim->cfg.time_to_debug);
		print_log(coder->sim, coder->id, "is refactoring");
		msleep(coder->sim->cfg.time_to_refactor);
	}
	return (NULL);
}
