/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destructor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:29:04 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 11:17:41 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	destroy_coders(t_coder *coders);

int	destroy_dongles(t_all *all)
{
	int	i;

	i = 0;
	while (i < all->params.nb_coders)
	{
		pthread_mutex_destroy(&all->dongles[i].mutex);
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
