/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 22:00:48 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:56:12 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void		*coders_loop(void *args);
void		init_thread_coders(t_coder *coders);
void		print_state(t_coder *coder, t_state state);
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
		coders[i].compile_count = 0;
		coders[i].last_compile = get_time(all);
		coders[i].is_finished = false;
		coders[i].deadline = 0;
		if (pthread_mutex_init(&coders[i].mutex, NULL))
		{
			destroy_dongles(all);
			return (0);
		}
		i++;
	}
	all->coders = coders;
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
	if (pthread_join(coders->all->monitor, NULL))
		return ;
}

void	*coders_loop(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	while (!is_running(coder->all))
		usleep(100);
	pthread_mutex_lock(&coder->mutex);
	coder->last_compile = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
	while (is_running(coder->all))
	{
		if (coder->id % 2 == 0)
		{
			lock_dongle(coder, coder->r_dongle);
			print_state(coder, take_dongle);
			lock_dongle(coder, coder->l_dongle);
			print_state(coder, take_dongle);
		}
		else
		{
			lock_dongle(coder, coder->l_dongle);
			print_state(coder, take_dongle);
			lock_dongle(coder, coder->r_dongle);
			print_state(coder, take_dongle);
		}
		coder_compile(coder);
		unlock_dongle(coder->all, coder->l_dongle);
		unlock_dongle(coder->all, coder->r_dongle);
		coder_debug(coder);
		coder_refactor(coder);
		if (coder->all->params.compiles_required > 0
			&& coder->compile_count >= coder->all->params.compiles_required)
		{
			pthread_mutex_lock(&coder->mutex);
			coder->is_finished = true;
			pthread_mutex_unlock(&coder->mutex);
			break ;
		}
	}
	return (NULL);
}
static void	coder_compile(t_coder *coder)
{
	print_state(coder, compile);
	ft_sleep(coder->all->params.compile, coder->all);
	pthread_mutex_lock(&coder->mutex);
	coder->compile_count++;
	coder->last_compile = get_time(coder->all);
	pthread_mutex_unlock(&coder->mutex);
}

static void	coder_debug(t_coder *coder)
{
	print_state(coder, debug);
	ft_sleep(coder->all->params.debug, coder->all);
}

static void	coder_refactor(t_coder *coder)
{
	print_state(coder, refactor);
	ft_sleep(coder->all->params.refactor, coder->all);
}

void	print_state(t_coder *coder, t_state state)
{
	long int	time;

	pthread_mutex_lock(&coder->all->message_mutex);
	time = get_time(coder->all);
	if (is_running(coder->all) || state == burned_out)
	{
		if (state == take_dongle)
			printf("%ld %d has taken a dongle\n", time, coder->id);
		else if (state == compile)
			printf("%ld %d is compiling\n", time, coder->id);
		else if (state == debug)
			printf("%ld %d is debugging\n", time, coder->id);
		else if (state == refactor)
			printf("%ld %d is refactoring\n", time, coder->id);
		else
			printf("%ld %d burned out\n", time, coder->id);
	}
	pthread_mutex_unlock(&coder->all->message_mutex);
}
