/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:26:28 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:42:53 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>
#include <pthread.h>
#include <stdbool.h>

int	set_memory(t_all *all)
{
	all->dongles = malloc(sizeof(t_dongle) * all->params.nb_coders + 1);
	if (!all->dongles)
		return (0);
	all->dongles = memset(all->dongles, 0, sizeof(t_dongle)
			* all->params.nb_coders + 1);
	return (1);
}
int	init_dongle(t_dongle *dongle, int nb_coders)
{
	dongle->available_at = 0;

	if (pthread_mutex_init(&dongle->mutex, NULL))
		return (0);

	if (!init_fifo(&dongle->fifo, nb_coders))
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}

	if (!init_heap(&dongle->edf, nb_coders))
	{
		pthread_mutex_destroy(&dongle->fifo.mutex);
		free(dongle->fifo.array);
		pthread_mutex_destroy(&dongle->mutex);
		return (0);
	}
	return (1);
}
int	init_dongles(t_all *all)
{
	int	i;

	i = 0;
	if (!set_memory(all))
	{
		destroy_all(all, all->coders);
		return (0);
	}
	while (i < all->params.nb_coders)
	{
		if (!init_dongle(&all->dongles[i], all->params.nb_coders)) {
			destroy_all(all, all->coders);
		
		}
		i++;
	}
	return (1);
}

int	init_all(t_all *all, t_coder *coders)
{
	if (pthread_mutex_init(&all->message_mutex, NULL))
	{
		destroy_all(all, coders);
		return (0);
	}
	if (pthread_mutex_init(&all->running_mutex, NULL))
	{
		destroy_all(all, coders);
		return (0);
	}
	if (pthread_create(&all->monitor, NULL, monitor_loop, all))
	{
		destroy_all(all, coders);
		return (0);
	}
	all->start_time = get_time(all);
	all->running = true;
	return (1);
}
