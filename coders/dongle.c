/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:56:30 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/05 21:03:57 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>

int	can_take_dongle(t_all *all, t_dongle *dongle)
{
	long int	current;
	int			available;

	pthread_mutex_lock(&dongle->mutex);
	current = get_time(all);
	if (all->params.dongle_cooldown + current >= dongle->available_at)
		available = 1;
	else
		available = 0;
	pthread_mutex_unlock(&dongle->mutex);
	return (available);
}

void	release_dongle(t_all *all, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->available_at = get_time(all) + all->params.dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}
