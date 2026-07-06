/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 20:56:30 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 11:23:40 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
int	can_take_dongle(t_all *all, t_dongle *dongle)
{
	return (get_time(all) >= dongle->available_at);
}

void	lock_dongle(t_all *all, t_dongle *dongle)
{
	while (is_running(all))
	{
		pthread_mutex_lock(&dongle->mutex);
		if (get_time(all) >= dongle->available_at)
			return ;
		pthread_mutex_unlock(&dongle->mutex);
		usleep(100);
	}
}

void	unlock_dongle(t_all *all, t_dongle *dongle)
{
	dongle->available_at =
		get_time(all) + all->params.dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}
