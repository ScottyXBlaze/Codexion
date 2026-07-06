/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 15:32:06 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 10:12:57 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long int	get_time(t_all *all)
{
	struct timeval	tv;
	long long		now;

	if (gettimeofday(&tv, NULL))
		return (-1);
	now = (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (now - all->start_time);
}

void	ft_sleep(long long sleep_time, t_all *all)
{
	long long	start;

	start = get_time(all);
	while ((get_time(all) - start) < sleep_time)
	{
		if (!is_running(all))
			break;
		usleep(500);
	}
}

bool	is_running(t_all *all)
{
	bool	status;

	pthread_mutex_lock(&all->running_mutex);
	status = all->running;
	pthread_mutex_unlock(&all->running_mutex);
	return (status);
}

void	stop_simulation(t_all *all)
{
	pthread_mutex_lock(&all->running_mutex);
	if (all->running)
		all->running = false;
	pthread_mutex_unlock(&all->running_mutex);
}
