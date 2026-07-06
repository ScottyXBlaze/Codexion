/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:29:04 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:44:13 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	destroy_coders(t_coder *coders);
void	destroy_dongle(t_dongle *dongle)
{
	pthread_mutex_destroy(&dongle->mutex);

	pthread_mutex_destroy(&dongle->fifo.mutex);
	free(dongle->fifo.array);

	pthread_mutex_destroy(&dongle->edf.mutex);
	free(dongle->edf.array);
}
int	destroy_dongles(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->params.nb_coders)
	{
		destroy_dongle(&all->dongles[i]);
		i++;
	}
	free(all->dongles);
	return (1);
}

int	destroy_all(t_all *all, t_coder *coders)
{
	pthread_mutex_destroy(&all->running_mutex);
	pthread_mutex_destroy(&all->message_mutex);
	destroy_dongles(all);
	destroy_coders(coders);
	return (1);
}

int	destroy_coders(t_coder *coders)
{
	free(coders);
	coders = NULL;
	return (1);
}
