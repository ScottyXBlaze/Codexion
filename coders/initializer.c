/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:26:28 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/05 21:04:57 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <limits.h>
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

int	init_dongles(t_all *all)
{
	int	i;

	i = 0;
	set_memory(all);
	while (i < all->params.nb_coders)
	{
		if (pthread_mutex_init(&all->dongles[i].mutex, NULL))
		{
			destroy_dongles(all);
			return (0);
		}
		if (pthread_cond_init(&all->dongles[i].cond, NULL))
		{
			destroy_dongles(all);
			return (0);
		}
		all->dongles[i].is_taken = false;
		all->dongles[i].available_at = INT_MIN;
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
	all->start_time = get_time(all);
	all->running = true;
	return (1);
}
