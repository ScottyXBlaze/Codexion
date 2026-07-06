/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 10:53:18 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 11:41:54 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdbool.h>

void	*monitor_loop(void *arg)
{
	t_all		*all;
	long int	last;
	bool		finished;
	bool		end_simulation;
	int			i;
	long int	now;

	all = (t_all *)arg;
	now = get_time(all);
	while (is_running(all))
	{
		i = 0;
		finished = true;
		end_simulation = true;
		while (i < all->params.nb_coders)
		{
			pthread_mutex_lock(&all->coders[i].mutex);
			last = all->coders[i].last_compile;
			finished = all->coders[i].is_finished;
			if (!finished)
				end_simulation = false;
			pthread_mutex_unlock(&all->coders[i].mutex);
			if (now - last > all->params.burnout && !finished)
			{
				stop_simulation(all);
				print_state(&all->coders[i], burned_out);
				return (NULL);
			}
			i++;
		}
		if (end_simulation)
		{
			stop_simulation(all);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}
