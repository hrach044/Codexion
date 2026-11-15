/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hmnatsak <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 16:15:06 by hmnatsak          #+#    #+#             */
/*   Updated: 2026/11/07 17:55:19 by hmnatsak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_dongle(t_dongle *d)
{
	pthread_mutex_init(&d->mutex, NULL);
	d->in_use = 0;
	d->free_at = 0;
}

int		try_take_dongle(t_dongle *d)
{
	int	taken;

	taken = 0;
	pthread_mutex_lock(&d->mutex);
	if  (!d->in_use && get_time() >= d->free_at)
	{
		d->in_use = 1;
		taken = 1;
	}
	pthread_mutex_unlock(&d->mutex);
	return (taken);
}

void	release_dongle(t_dongle *d, long cooldown)
{
	pthread_mutex_lock(&d->mutex);
	d->in_use = 0;
	d->free_at = get_time() + cooldown;
	pthread_mutex_unlock(&d->mutex);
}
