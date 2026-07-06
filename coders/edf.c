/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nyramana <nyramana@student.42antananarivo  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/06 12:07:35 by nyramana          #+#    #+#             */
/*   Updated: 2026/07/06 12:56:43 by nyramana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "codexion.h"

void		heap_push(t_heap *heap, t_coder *coder);
t_coder		*heap_peek(t_heap *heap);
void		heap_pop(t_heap *heap);

static long int	coder_deadline(t_coder *coder)
{
	long int	last_compile;

	pthread_mutex_lock(&coder->mutex);
	last_compile = coder->last_compile;
	pthread_mutex_unlock(&coder->mutex);
	return (last_compile + coder->all->params.burnout);
}

static int	is_higher_priority(t_coder *a, t_coder *b)
{
	long int	a_deadline;
	long int	b_deadline;

	a_deadline = coder_deadline(a);
	b_deadline = coder_deadline(b);
	if (a_deadline != b_deadline)
		return (a_deadline < b_deadline);
	return (a->id < b->id);
}

int	init_heap(t_heap *heap, int capacity)
{
	heap->array = malloc(sizeof(t_coder *) * capacity);
	if (!heap->array)
		return (0);

	heap->size = 0;
	heap->capacity = capacity;

	if (pthread_mutex_init(&heap->mutex, NULL))
	{
		free(heap->array);
		return (0);
	}
	return (1);
}

void	destroy_heap(t_heap *heap)
{
	free(heap->array);
	pthread_mutex_destroy(&heap->mutex);
}

void	heap_push(t_heap *heap, t_coder *coder)
{
	int	index;
	int	parent;

	pthread_mutex_lock(&heap->mutex);
	if (heap->size >= heap->capacity)
	{
		pthread_mutex_unlock(&heap->mutex);
		return ;
	}
	index = heap->size;
	heap->array[index] = coder;
	heap->size++;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!is_higher_priority(heap->array[index], heap->array[parent]))
			break ;
		coder = heap->array[index];
		heap->array[index] = heap->array[parent];
		heap->array[parent] = coder;
		index = parent;
	}
	pthread_mutex_unlock(&heap->mutex);
}

t_coder	*heap_peek(t_heap *heap)
{
	t_coder	*coder;

	pthread_mutex_lock(&heap->mutex);
	coder = NULL;
	if (heap->size > 0)
		coder = heap->array[0];
	pthread_mutex_unlock(&heap->mutex);
	return (coder);
}

void	heap_pop(t_heap *heap)
{
	int		index;
	int		left;
	int		right;
	int		smallest;
	t_coder	*tmp;

	pthread_mutex_lock(&heap->mutex);
	if (heap->size <= 0)
	{
		pthread_mutex_unlock(&heap->mutex);
		return ;
	}
	heap->size--;
	if (heap->size == 0)
	{
		pthread_mutex_unlock(&heap->mutex);
		return ;
	}
	heap->array[0] = heap->array[heap->size];
	index = 0;
	while (1)
	{
		left = (index * 2) + 1;
		right = (index * 2) + 2;
		smallest = index;
		if (left < heap->size
			&& is_higher_priority(heap->array[left], heap->array[smallest]))
			smallest = left;
		if (right < heap->size
			&& is_higher_priority(heap->array[right], heap->array[smallest]))
			smallest = right;
		if (smallest == index)
			break ;
		tmp = heap->array[index];
		heap->array[index] = heap->array[smallest];
		heap->array[smallest] = tmp;
		index = smallest;
	}
	pthread_mutex_unlock(&heap->mutex);
}

void	lock_dongle_edf(t_coder *coder, t_dongle *dongle)
{
	long int	remaining_cooldown;

	heap_push(&dongle->edf, coder);
	while (is_running(coder->all))
	{
		if (heap_peek(&dongle->edf) == coder)
		{
			pthread_mutex_lock(&dongle->mutex);
			if (can_take_dongle(coder->all, dongle))
			{
				heap_pop(&dongle->edf);
				return ;
			}
			remaining_cooldown = dongle->available_at - get_time(coder->all);
			pthread_mutex_unlock(&dongle->mutex);
			if (remaining_cooldown > 0)
			{
				if (remaining_cooldown > 1)
					usleep(1000);
				else
					usleep(remaining_cooldown * 1000);
				continue ;
			}
		}
		usleep(100);
	}
}
