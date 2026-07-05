/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/05 15:32:06 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/05 15:43:25 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long int	get_time(t_all *all)
{
	long long		timestamp;
	struct timeval	current;
	long			current_ms;

	timestamp = all->start_time;
	if (gettimeofday(&current, NULL))
		return (-1);
	current_ms = (long long)current.tv_sec * 1000 + current.tv_usec / 1000;
	return (current_ms - timestamp);
}

void	ft_sleep(long long sleep_time, t_all *all)
{
	long long	start;

	start = get_time(all);
	while ((get_time(all) - start) < sleep_time)
		usleep(100);
}
