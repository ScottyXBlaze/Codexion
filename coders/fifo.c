/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fifo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 11:46:28 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:59:35 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "codexion.h"

void	fifo_push(t_fifo *fifo, t_coder *coder);
t_coder	*fifo_front(t_fifo *fifo);
void	fifo_pop(t_fifo *fifo);

int	init_fifo(t_fifo *fifo, int capacity)
{
	fifo->array = malloc(sizeof(t_coder *) * capacity);
	if (!fifo->array)
		return (0);
	fifo->head = 0;
	fifo->tail = 0;
	fifo->size = 0;
	fifo->capacity = capacity;
	if (pthread_mutex_init(&fifo->mutex, NULL))
	{
		free(fifo->array);
		return (0);
	}
	return (1);
}

void	destroy_fifo(t_fifo *fifo)
{
	free(fifo->array);
	pthread_mutex_destroy(&fifo->mutex);
}

void	fifo_push(t_fifo *fifo, t_coder *coder)
{
	pthread_mutex_lock(&fifo->mutex);
	if (fifo->size < fifo->capacity)
	{
		fifo->array[fifo->tail] = coder;
		fifo->tail = (fifo->tail + 1) % fifo->capacity;
		fifo->size++;
	}
	pthread_mutex_unlock(&fifo->mutex);
}

t_coder	*fifo_front(t_fifo *fifo)
{
	t_coder	*coder;

	pthread_mutex_lock(&fifo->mutex);
	coder = NULL;
	if (fifo->size > 0)
		coder = fifo->array[fifo->head];
	pthread_mutex_unlock(&fifo->mutex);
	return (coder);
}

void	fifo_pop(t_fifo *fifo)
{
	pthread_mutex_lock(&fifo->mutex);
	if (fifo->size > 0)
	{
		fifo->head = (fifo->head + 1) % fifo->capacity;
		fifo->size--;
	}
	pthread_mutex_unlock(&fifo->mutex);
}

void	lock_dongle_fifo(t_coder *coder, t_dongle *dongle)
{
	fifo_push(&dongle->fifo, coder);

	while (is_running(coder->all))
	{
		if (fifo_front(&dongle->fifo) == coder)
		{
			pthread_mutex_lock(&dongle->mutex);

			if (get_time(coder->all)
				>= dongle->available_at)
			{
				fifo_pop(&dongle->fifo);
				pthread_mutex_unlock(&dongle->mutex);
				return ;
			}
			pthread_mutex_unlock(&dongle->mutex);
		}
		usleep(100);
	}
}
