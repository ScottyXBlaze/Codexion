/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 22:00:48 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/02 23:01:37 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "codexion.h"

void	*coders_loop(void *args);
void	init_thread_coders(t_coder *coders);

t_coder	*init_coders(t_all *all, t_coder *coders)
{
	int	i;

	i = 0;
	coders = malloc(sizeof(t_coder) * all->params.nb_coders);
	if (coders == NULL)
	{
		destroy_all(all, coders);
		return (NULL);
	}
	coders = memset(coders, 0, sizeof(t_coder) * all->params.nb_coders);
	while (i < all->params.nb_coders)
	{
		printf("Coder %d created\n", i + 1);
		coders[i].id = i + 1;
		coders[i].all = all;
		coders[i].l_dongle = all->dongles[i];
		coders[i].r_dongle = all->dongles[(i + 1) % all->params.nb_coders];
		coders[i].nb_activity = 0;
		coders[i].time = 0.0;
		i++;
	}
	init_thread_coders(coders);
	return (coders);
}

void	init_thread_coders(t_coder *coders)
{
	int	i;
	int nb_coders;

	i = 0;
	nb_coders = coders[0].all->params.nb_coders;
	
	while (i < nb_coders)
	{
		if (pthread_create(&coders[i].thread, NULL, coders_loop, &coders[i]))
			return ;
		i++;
	}
	i = 0;
	while (i < nb_coders)
	{
		if (pthread_join(coders[i].thread, NULL))
			return ;
		i++;
	}
}

void	*coders_loop(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	while (coder->all->running)
	{
		printf("HEY");
	}
	return (NULL);
}
