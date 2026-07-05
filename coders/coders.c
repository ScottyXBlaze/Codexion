/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 22:00:48 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/05 21:13:45 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void		*coders_loop(void *args);
void		init_thread_coders(t_coder *coders);
void		print_state(int time, int id, int state);
static void	coder_compile(t_coder *coder);
static void	coder_debug(t_coder *coder);
static void	coder_refactor(t_coder *coder);

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
		coders[i].id = i + 1;
		coders[i].all = all;
		coders[i].l_dongle = &all->dongles[i];
		coders[i].r_dongle = &all->dongles[(i + 1) % all->params.nb_coders];
		coders[i].nb_activity = 0;
		coders[i].time = 0.0;
		if (pthread_mutex_init(&coders[i].mutex, NULL))
		{
			destroy_dongles(all);
			return (0);
		}
		i++;
	}
	init_thread_coders(coders);
	return (coders);
}

void	init_thread_coders(t_coder *coders)
{
	int	i;
	int	nb_coders;

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
	while (!coder->all->running)
		usleep(100);
	pthread_mutex_lock(&coder->mutex);
	coder->time = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
	while (1)
	{
		if (!coder->all->running)
		{
			pthread_mutex_unlock(&coder->all->running_mutex);
			break ;
		}
		if (!coder->id % 2)
		{
			pthread_mutex_lock(&coder->r_dongle->mutex);
			pthread_mutex_lock(&coder->l_dongle->mutex);
		}
		else
		{
			pthread_mutex_lock(&coder->l_dongle->mutex);
			pthread_mutex_lock(&coder->r_dongle->mutex);
		}
		coder_compile(coder);
		pthread_mutex_unlock(&coder->l_dongle->mutex);
		pthread_mutex_unlock(&coder->r_dongle->mutex);
		coder_debug(coder);
		coder_refactor(coder);
		if (coder->all->params.compiles_required < coder->nb_activity)
			break ;
	}
	return (NULL);
}

static void	coder_compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->all->message_mutex);
	print_state(get_time(coder->all), coder->id, 0);
	print_state(get_time(coder->all), coder->id, 0);
	print_state(get_time(coder->all), coder->id, 1);
	pthread_mutex_unlock(&coder->all->message_mutex);
	ft_sleep(coder->all->params.compile, coder->all);
	pthread_mutex_lock(&coder->mutex);
	coder->nb_activity++;
	coder->time = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
}

static void	coder_debug(t_coder *coder)
{
	pthread_mutex_lock(&coder->all->message_mutex);
	print_state(get_time(coder->all), coder->id, 2);
	pthread_mutex_unlock(&coder->all->message_mutex);
	ft_sleep(coder->all->params.debug, coder->all);
}

static void	coder_refactor(t_coder *coder)
{
	pthread_mutex_lock(&coder->all->message_mutex);
	print_state(get_time(coder->all), coder->id, 3);
	pthread_mutex_unlock(&coder->all->message_mutex);
	ft_sleep(coder->all->params.refactor, coder->all);
}

void	print_state(int time, int id, int state)
{
	if (state == 0)
		printf("[%d] %d has taken a dongle\n", time, id);
	if (state == 1)
		printf("[%d] %d is compiling\n", time, id);
	if (state == 2)
		printf("[%d] %d is debugging\n", time, id);
	if (state == 3)
		printf("[%d] %d is refactoring\n", time, id);
	if (state == 4)
		printf("[%d] %d burned out\n", time, id);
}
